//! Native reference: the isolated interpreter and TRON energy schedule from our Rust node.
use crate::{
    energy,
    model::{Call, Outcome},
};
use anyhow::{bail, Result};
use revm_context::{BlockEnv, CfgEnv, Context, Journal, TxEnv};
use revm_context_interface::{Database, TronDatabaseExt};
use revm_interpreter::{
    instruction_table,
    interpreter::{EthInterpreter, ExtBytecode},
    primitives::{hardfork::SpecId, Address, Bytes, B256, U256},
    state::{AccountInfo, Bytecode},
    CallInput, Host, InputsImpl, Interpreter, InterpreterAction, SharedMemory,
};
use std::{collections::BTreeMap, convert::Infallible};

pub const CONTRACT: Address = Address::new([2; 20]);
pub const CALLER: Address = Address::new([1; 20]);

#[derive(Clone, Debug, Default)]
pub struct MemoryDb {
    pub storage: BTreeMap<U256, U256>,
}
impl Database for MemoryDb {
    type Error = Infallible;
    fn basic(&mut self, _: Address) -> std::result::Result<Option<AccountInfo>, Self::Error> {
        Ok(Some(AccountInfo::default()))
    }
    fn code_by_hash(&mut self, _: B256) -> std::result::Result<Bytecode, Self::Error> {
        Ok(Bytecode::default())
    }
    fn storage(&mut self, _: Address, key: U256) -> std::result::Result<U256, Self::Error> {
        Ok(self.storage.get(&key).copied().unwrap_or_default())
    }
    fn block_hash(&mut self, _: u64) -> std::result::Result<B256, Self::Error> {
        Ok(B256::ZERO)
    }
}
impl TronDatabaseExt for MemoryDb {
    fn tron_allow_multi_sign(&self) -> bool {
        true
    }
    fn tron_allow_tvm_solidity_059(&self) -> bool {
        true
    }
    fn tron_allow_tvm_constantinople(&self) -> bool {
        true
    }
    fn tron_allow_tvm_compatible_evm(&self) -> bool {
        true
    }
    fn tron_energy_fee(&self) -> i64 {
        100
    }
}
pub type MemoryContext = Context<BlockEnv, TxEnv, CfgEnv, MemoryDb, Journal<MemoryDb>>;

pub fn context(initial: BTreeMap<U256, U256>) -> MemoryContext {
    let mut ctx = MemoryContext::new(MemoryDb { storage: initial }, SpecId::CANCUN);
    ctx.cfg.gas_params = energy::tron_gas_params_for(false);
    ctx.cfg.chain_id = 728126428;
    ctx.tx.caller = CALLER;
    ctx.block.number = U256::from(1);
    ctx.block.timestamp = U256::from(1_700_000_000u64);
    let _ = ctx.balance(CONTRACT);
    ctx
}
pub fn storage_state(ctx: &MemoryContext) -> BTreeMap<String, String> {
    let mut result = ctx.journaled_state.database.storage.clone();
    if let Some(account) = ctx.journaled_state.state.get(&CONTRACT) {
        for (k, v) in &account.storage {
            result.insert(*k, v.present_value());
        }
    }
    result
        .into_iter()
        .filter(|(_, v)| !v.is_zero())
        .map(|(k, v)| (format!("{k:064x}"), format!("{v:064x}")))
        .collect()
}
pub fn initial_state(call: &Call) -> BTreeMap<String, String> {
    call.initial_storage
        .iter()
        .filter(|(_, v)| !v.is_zero())
        .map(|(k, v)| (format!("{k:064x}"), format!("{v:064x}")))
        .collect()
}
pub struct NativeRunner {
    pub bytecode: Bytecode,
}
impl NativeRunner {
    pub fn new(code: &[u8]) -> Self {
        Self {
            bytecode: Bytecode::new_legacy(Bytes::copy_from_slice(code)),
        }
    }
    pub fn run(&self, call: &Call) -> Result<Outcome> {
        crate::model::validate(call)?;
        let mut ctx = context(call.initial_storage.clone());
        let mut interpreter = Interpreter::<EthInterpreter>::new(
            SharedMemory::new_with_memory_limit(3 * 1024 * 1024),
            ExtBytecode::new(self.bytecode.clone()),
            InputsImpl {
                target_address: CONTRACT,
                bytecode_address: Some(CONTRACT),
                caller_address: CALLER,
                input: CallInput::Bytes(Bytes::copy_from_slice(&call.input)),
                ..Default::default()
            },
            false,
            SpecId::CANCUN,
            call.energy_limit,
        );
        interpreter.gas.set_tron_dynamic_factor(call.dynamic_factor);
        let action = interpreter.run_plain(
            &instruction_table(),
            &energy::tron_static_gas_table(true),
            &mut ctx,
        );
        let result = match action {
            InterpreterAction::Return(r) => r,
            _ => bail!("nested CALL/CREATE not yet supported by the POC frame driver"),
        };
        let kind = format!("{:?}", result.result);
        let status = match kind.as_str() {
            "Stop" | "Return" | "SelfDestruct" => "success",
            "Revert" => "revert",
            x if x.contains("OutOfGas") || x.contains("OOG") => "out_of_energy",
            "StackUnderflow" => "stack_underflow",
            "StackOverflow" => "stack_overflow",
            "InvalidJump" => "bad_jump",
            "OpcodeNotFound" | "InvalidFEOpcode" => "invalid",
            _ => "exception",
        }
        .to_string();
        let success = status == "success";
        let used = if success || status == "revert" {
            call.energy_limit - result.gas.remaining()
        } else {
            call.energy_limit
        };
        Ok(Outcome {
            status,
            energy_used: used,
            output: hex::encode(result.output),
            storage: if success {
                storage_state(&ctx)
            } else {
                initial_state(call)
            },
            logs: if success {
                ctx.journaled_state
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
