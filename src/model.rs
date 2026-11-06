use revm_interpreter::primitives::U256;
use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;

#[derive(Clone, Debug)]
pub struct Call {
    pub input: Vec<u8>,
    pub energy_limit: u64,
    pub dynamic_factor: i64,
    pub initial_storage: BTreeMap<U256, U256>,
}
impl Default for Call {
    fn default() -> Self {
        Self {
            input: vec![],
            energy_limit: 100_000_000,
            dynamic_factor: 0,
            initial_storage: BTreeMap::new(),
        }
    }
}
#[derive(Clone, Debug, PartialEq, Eq, Serialize, Deserialize)]
pub struct Outcome {
    pub status: String,
    pub energy_used: u64,
    pub output: String,
    pub storage: BTreeMap<String, String>,
    pub logs: Vec<String>,
}

pub fn validate(call: &Call) -> anyhow::Result<()> {
    anyhow::ensure!(call.input.len() <= 65536, "POC calldata limit is 64 KiB");
    anyhow::ensure!(
        call.energy_limit <= i64::MAX as u64,
        "POC energy limit exceeds signed 64-bit range"
    );
    anyhow::ensure!(
        (0..=1_000_000).contains(&call.dynamic_factor),
        "POC dynamic factor must be 0..=1,000,000"
    );
    Ok(())
}
