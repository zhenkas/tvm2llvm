// SPDX-License-Identifier: LGPL-3.0-or-later
use crate::{
    model::{Call, Outcome},
    native::{self, MemoryContext, CONTRACT},
};
use anyhow::{bail, Result};
use revm_interpreter::{
    primitives::{keccak256, Log, B256, U256},
    Host,
};
use std::{path::Path, time::Instant};
use wasmtime::{Caller, Engine, Linker, Memory, Module, Store, TypedFunc};
struct State {
    ctx: MemoryContext,
}
fn mem(c: &mut Caller<'_, State>) -> Memory {
    c.get_export("memory").unwrap().into_memory().unwrap()
}
fn word(c: &mut Caller<'_, State>, p: i32) -> U256 {
    let mut b = [0; 32];
    mem(c).read(&*c, p as usize, &mut b).unwrap();
    U256::from_le_bytes(b)
}
fn bytes(c: &mut Caller<'_, State>, p: i32, n: i32) -> Vec<u8> {
    let mut b = vec![0; n as usize];
    mem(c).read(&*c, p as usize, &mut b).unwrap();
    b
}
fn put(c: &mut Caller<'_, State>, p: i32, w: U256) {
    mem(c)
        .write(&mut *c, p as usize, &w.to_le_bytes::<32>())
        .unwrap();
}
pub struct WasmRunner {
    store: Store<State>,
    memory: Memory,
    execute: TypedFunc<(i32, i64, i64), i32>,
    input: i32,
    remaining: TypedFunc<(), i64>,
    outptr: TypedFunc<(), i32>,
    outlen: TypedFunc<(), i32>,
    pub jit_ms: f64,
}
impl WasmRunner {
    pub fn new(path: &Path) -> Result<Self> {
        let t = Instant::now();
        let engine = Engine::default();
        let module = Module::from_file(&engine, path)?;
        let mut linker = Linker::new(&engine);
        linker.func_wrap(
            "host",
            "sload",
            |mut c: Caller<'_, State>, k: i32, v: i32| {
                let key = word(&mut c, k);
                let val = c.data_mut().ctx.sload(CONTRACT, key).unwrap().data;
                put(&mut c, v, val);
            },
        )?;
        linker.func_wrap(
            "host",
            "sstore",
            |mut c: Caller<'_, State>, k: i32, v: i32| -> i64 {
                let key = word(&mut c, k);
                let val = word(&mut c, v);
                let load = c.data_mut().ctx.sstore(CONTRACT, key, val).unwrap();
                c.data()
                    .ctx
                    .cfg
                    .gas_params
                    .sstore_dynamic_gas(false, &load.data, load.is_cold) as i64
            },
        )?;
        linker.func_wrap(
            "host",
            "keccak",
            |mut c: Caller<'_, State>, p: i32, n: i32, v: i32| {
                let data = bytes(&mut c, p, n);
                put(&mut c, v, U256::from_be_bytes(keccak256(data).0));
            },
        )?;
        linker.func_wrap(
            "host",
            "log",
            |mut c: Caller<'_, State>, p: i32, n: i32, t: i32, count: i32| {
                let data = bytes(&mut c, p, n);
                let topics = (0..count)
                    .map(|i| B256::from(word(&mut c, t + 32 * i).to_be_bytes::<32>()))
                    .collect();
                c.data_mut()
                    .ctx
                    .log(Log::new_unchecked(CONTRACT, topics, data.into()));
            },
        )?;
        let mut store = Store::new(
            &engine,
            State {
                ctx: native::context(Default::default()),
            },
        );
        let inst = linker.instantiate(&mut store, &module)?;
        let memory = inst.get_memory(&mut store, "memory").unwrap();
        let execute = inst.get_typed_func(&mut store, "execute")?;
        let input = inst
            .get_typed_func::<(), i32>(&mut store, "input_ptr")?
            .call(&mut store, ())?;
        let remaining = inst.get_typed_func(&mut store, "remaining_energy")?;
        let outptr = inst.get_typed_func(&mut store, "output_ptr")?;
        let outlen = inst.get_typed_func(&mut store, "output_len")?;
        Ok(Self {
            store,
            memory,
            execute,
            input,
            remaining,
            outptr,
            outlen,
            jit_ms: t.elapsed().as_secs_f64() * 1000.,
        })
    }
    pub fn run(&mut self, call: &Call) -> Result<Outcome> {
        crate::model::validate(call)?;
        if call.input.len() > 65536 {
            bail!("calldata exceeds POC 64 KiB limit");
        }
        if call.energy_limit > i64::MAX as u64 {
            bail!("energy outside POC range");
        }
        self.store.data_mut().ctx = native::context(call.initial_storage.clone());
        self.memory
            .write(&mut self.store, self.input as usize, &call.input)?;
        let code = self.execute.call(
            &mut self.store,
            (
                call.input.len() as i32,
                call.energy_limit as i64,
                call.dynamic_factor,
            ),
        )?;
        let status = match code {
            0 => "success",
            1 => "revert",
            2 => "out_of_energy",
            3 => "invalid",
            4 => "stack_underflow",
            5 => "stack_overflow",
            6 => "bad_jump",
            7 => "exception",
            8 => bail!("unsupported opcode reached in compiled fixture"),
            _ => bail!("unknown Wasm status {code}"),
        }
        .to_string();
        let remaining = self.remaining.call(&mut self.store, ())? as u64;
        let p = self.outptr.call(&mut self.store, ())? as usize;
        let n = self.outlen.call(&mut self.store, ())? as usize;
        let mut out = vec![0; n];
        self.memory.read(&self.store, p, &mut out)?;
        let success = code == 0;
        Ok(Outcome {
            status,
            energy_used: if code <= 1 {
                call.energy_limit - remaining
            } else {
                call.energy_limit
            },
            output: hex::encode(out),
            storage: if success {
                native::storage_state(&self.store.data().ctx)
            } else {
                native::initial_state(call)
            },
            logs: if success {
                self.store
                    .data()
                    .ctx
                    .journaled_state
                    .logs
                    .iter()
                    .map(|l| format!("{l:?}"))
                    .collect()
            } else {
                vec![]
            },
        })
    }
}
