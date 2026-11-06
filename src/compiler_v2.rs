// SPDX-License-Identifier: LGPL-3.0-or-later
//! Experimental v2: outlined opcode reservation and one C++ exception boundary.
use crate::energy;
use anyhow::{bail, Context, Result};
use serde::Serialize;
use std::{
    fmt::Write as _,
    path::{Path, PathBuf},
    process::Command,
    time::Instant,
};
#[derive(Debug, Serialize)]
pub struct Compilation {
    pub cpp: PathBuf,
    pub wasm: PathBuf,
    pub generation_ms: f64,
    pub clang_ms: f64,
    pub wasm_bytes: u64,
}
fn literal(bytes: &[u8]) -> String {
    let mut b = [0u8; 32];
    b[32 - bytes.len()..].copy_from_slice(bytes);
    let v: Vec<_> = b
        .chunks_exact(8)
        .rev()
        .map(|x| format!("{}ULL", u64::from_be_bytes(x.try_into().unwrap())))
        .collect();
    format!("Word{{{{{}}}}}", v.join(","))
}
pub fn generate(code: &[u8]) -> String {
    let mut ins = vec![];
    let mut pc = 0;
    while pc < code.len() {
        let op = code[pc];
        let n = if (0x60..=0x7f).contains(&op) {
            usize::from(op - 0x5f)
        } else {
            0
        };
        let mut data = vec![0; n];
        let end = (pc + 1 + n).min(code.len());
        data[..end - pc - 1].copy_from_slice(&code[pc + 1..end]);
        ins.push((pc, op, data));
        pc += 1 + n;
    }
    let gas = energy::tron_static_gas_table(true);
    let mut s = String::from("#include \"runtime.hpp\"\nstatic const uint8_t CODE[]={");
    for x in code {
        write!(s, "{x},").unwrap();
    }
    if code.is_empty() {
        s.push('0');
    }
    writeln!(s,"}};\nextern \"C\" __attribute__((visibility(\"default\"))) uint32_t execute(uint32_t len,uint64_t energy,int64_t factor){{c.reset(len,energy,factor);if(len>65536)return UNSUPPORTED;try{{goto P0;").unwrap();
    for (pc, op, data) in &ins {
        let next = pc + 1 + data.len();
        writeln!(s, "P{pc}:{{").unwrap();
        let (n, d) = match op {
            0x01..=0x07 | 0x0a..=0x0b | 0x10..=0x14 | 0x16..=0x18 | 0x1a..=0x1d => (2, -1),
            0x08..=0x09 => (3, -2),
            0x15 | 0x19 | 0x35 | 0x51 | 0x54 => (1, 0),
            0x20 => (2, -1),
            0x37 | 0x39 | 0x3e => (3, -3),
            0x50 | 0x56 => (1, -1),
            0x52 | 0x53 | 0x55 | 0x57 | 0xf3 | 0xfd => (2, -2),
            0x80..=0x8f => (usize::from(*op - 0x7f), 1),
            0x90..=0x9f => (usize::from(*op - 0x8f) + 1, 0),
            0xa0..=0xa4 => (usize::from(*op - 0xa0) + 2, -i32::from(*op - 0xa0) - 2),
            0x30
            | 0x32..=0x34
            | 0x36
            | 0x38
            | 0x3a
            | 0x3d
            | 0x41..=0x48
            | 0x58..=0x5a
            | 0x5f..=0x7f => (0, 1),
            _ => (0, 0),
        };
        writeln!(s, "reserve<{op}>({}, {n}, {d});", gas[*op as usize]).unwrap();
        let body = match op {
            0x00 => "return SUCCESS;".into(),
            0x01..=0x07 | 0x0a..=0x0b | 0x10..=0x14 | 0x16..=0x18 | 0x1a..=0x1d => {
                format!("binary<{op}>();")
            }
            0x08..=0x09 => format!("ternary<{op}>();"),
            0x15 | 0x19 => format!("unary<{op}>();"),
            0x20 => "hash_op();".into(),
            0x30 | 0x32 | 0x33 => {
                let n = if *op == 0x30 { 2 } else { 1 };
                let mut b = [n; 32];
                b[..12].fill(0);
                // Modern MULTI_SIGN fixture mode exposes 20-byte addresses.
                b[11] = 0;
                format!("c.push({});", literal(&b))
            }
            0x34 | 0x3a | 0x3d | 0x41 | 0x44 | 0x45 | 0x47 => "c.push(Word{});".into(),
            0x35 => "calldata_load();".into(),
            0x36 => "c.push(Word::small(c.input_len));".into(),
            0x38 => format!("c.push(Word::small({}));", code.len()),
            0x37 | 0x39 | 0x3e => format!("copy_op<{op}>(CODE,{});", code.len()),
            0x42 => "c.push(Word::small(1700000000));".into(),
            0x43 => "c.push(Word::small(1));".into(),
            0x46 => "c.push(Word::small(728126428));".into(),
            0x48 => "c.push(Word::small(100));".into(),
            0x50 => "c.pop();".into(),
            0x51..=0x53 => format!("memory_op<{op}>();"),
            0x54 => format!("storage_op<{op}>();"),
            0x55 => format!(
                "c.charge({});storage_op<{op}>();",
                energy::tron_gas_params_for(false).sstore_static_gas()
            ),
            0x56 => "c.jump=c.pop();goto DISPATCH;".into(),
            0x57 => "c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;".into(),
            0x58 => format!("c.push(Word::small({pc}));"),
            0x59 => "c.push(Word::small(c.mem_size));".into(),
            0x5a => "c.push(Word::small(c.remaining));".into(),
            0x5b => String::new(),
            0x5f => "c.push(Word{});".into(),
            0x60..=0x7f => format!("c.push({});", literal(data)),
            0x80..=0x8f => format!("c.push(c.stack[c.sp-{}]);", op - 0x7f),
            0x90..=0x9f => format!(
                "{{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-{}];c.stack[c.sp-{}]=t;}}",
                op - 0x8f + 1,
                op - 0x8f + 1
            ),
            0xa0..=0xa4 => format!(
                "c.charge({});log_op<{}>();",
                energy::tron_gas_params_for(false).log_cost(op - 0xa0, 0),
                op - 0xa0
            ),
            0xf3 => "finish(SUCCESS);return c.status;".into(),
            0xfd => "finish(REVERTED);return c.status;".into(),
            0xfe => "return INVALID;".into(),
            _ => "return UNSUPPORTED;".into(),
        };
        writeln!(s, "{body}\ngoto P{next};}}").unwrap();
    }
    if code.is_empty() {
        s.push_str("P0: return SUCCESS;\n");
    } else {
        writeln!(s, "P{pc}: return SUCCESS;").unwrap();
    }
    s.push_str("DISPATCH: if(c.jump.high())return BAD_JUMP;switch(c.jump.v[0]){\n");
    for (pc, op, _) in &ins {
        if *op == 0x5b {
            writeln!(s, "case {pc}:goto P{pc};").unwrap();
        }
    }
    s.push_str("default:return BAD_JUMP;}}catch(const VmExit& e){++caught;return e.status;}}\n");
    s
}
fn run(cmd: &mut Command) -> Result<()> {
    let r = cmd.output()?;
    if !r.status.success() {
        bail!("compiler failed: {}", String::from_utf8_lossy(&r.stderr));
    }
    Ok(())
}
pub fn compile(code: &[u8], dir: &Path, name: &str) -> Result<Compilation> {
    std::fs::create_dir_all(dir)?;
    let t = Instant::now();
    let cpp = dir.join(format!("{name}.cpp"));
    let wasm = dir.join(format!("{name}.wasm"));
    let obj = dir.join(format!("{name}.o"));
    std::fs::write(
        dir.join("runtime.hpp"),
        include_str!("../runtime/runtime_v2.hpp"),
    )?;
    std::fs::write(&cpp, generate(code))?;
    let generation_ms = t.elapsed().as_secs_f64() * 1000.;
    let t = Instant::now();
    run(Command::new(
        std::env::var("TVM_CLANG_V2")
            .or_else(|_| std::env::var("TVM_CLANG"))
            .unwrap_or("clang++".into()),
    )
    .args([
        "--target=wasm32-wasip1",
        "-std=c++17",
        "-O3",
        "-ffreestanding",
        "-fno-builtin",
        "-fwasm-exceptions",
        "-mllvm",
        "-wasm-use-legacy-eh=false",
        "-fno-rtti",
        "-fvisibility=hidden",
        "-c",
    ])
    .arg(&cpp)
    .arg("-o")
    .arg(&obj))?;
    let clang = std::env::var("TVM_CLANG_V2")
        .or_else(|_| std::env::var("TVM_CLANG"))
        .unwrap_or("clang++".into());
    run(Command::new(clang)
        .arg(&obj)
        .args([
            "--target=wasm32-wasip1",
            "-fwasm-exceptions",
            "-mllvm",
            "-wasm-use-legacy-eh=false",
            "-lunwind",
            "-Wl,--no-entry",
            "-Wl,--export=execute",
            "-Wl,--export=input_ptr",
            "-Wl,--export=remaining_energy",
            "-Wl,--export=output_ptr",
            "-Wl,--export=output_len",
            "-Wl,--export=caught_exceptions",
            "-Wl,--export=__wasm_call_ctors",
            "-Wl,--export-memory",
            "-Wl,--initial-memory=4194304",
            "-Wl,--max-memory=4194304",
            "-Wl,-z,stack-size=262144",
            "-o",
        ])
        .arg(&wasm))
    .context("linking exception-enabled Wasm (wasi-sdk 34 required)")?;
    Ok(Compilation {
        cpp,
        wasm: wasm.clone(),
        generation_ms,
        clang_ms: t.elapsed().as_secs_f64() * 1000.,
        wasm_bytes: std::fs::metadata(wasm)?.len(),
    })
}
