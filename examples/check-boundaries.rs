// SPDX-License-Identifier: LGPL-3.0-or-later
//! Differential edge cases for grouped resource checks and dynamic jump boundaries.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::U256;
use serde_json::json;
use std::path::PathBuf;
use tvm2llvm::{compiler, model::Call, native::NativeRunner, wasm::WasmRunner};
fn main() -> Result<()> {
    let dir =
        PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts/boundaries".into()));
    std::fs::create_dir_all(&dir)?;
    let mut overflow = vec![0x5f; 1025];
    overflow.push(0);
    let mut high_jump = vec![0x7f];
    high_jump.extend([0xff; 32]);
    high_jump.push(0x56);
    let mut cases: Vec<(&str, Vec<u8>, Vec<u8>)> = vec![
        ("stack-overflow", overflow, vec![]),
        ("high-jump", high_jump, vec![]),
    ];
    for (name, code) in [
        ("charge-before-underflow", "60010100"),
        ("mixed-cost-rounding", "600180015f5000"),
        ("gas-observation", "60016002015a60005260206000f3"),
        ("jump-to-destination", "600456005b602a60005260206000f3"),
        ("jump-into-push-data", "605b600156"),
        ("false-jump-invalid-target", "600060015700"),
        ("fallthrough-end", "6001600201"),
        ("unreachable-invalid", "00fe"),
        ("dynamic-exp", "602060020a60005260206000f3"),
        ("out-of-range-memory", "6300300000600052"),
        ("storage-rollback", "602a6000556300300000600052"),
        ("log-rollback", "60006000a06300300000600052"),
        ("returndata-bounds", "6001600060003e"),
    ] {
        cases.push((name, hex::decode(code)?, vec![]));
    }
    let conditional = hex::decode("600035600a57600700005b602a00")?;
    cases.push(("conditional-false", conditional.clone(), vec![0; 32]));
    let mut data = vec![0; 32];
    data[31] = 1;
    cases.push(("conditional-true", conditional, data));
    let mut checks = 0;
    let mut rows = vec![];
    for (name, code, input) in cases {
        let native = NativeRunner::new(&code);
        let compiled = compiler::compile(&code, &dir, name)?;
        let mut vm = WasmRunner::new(&compiled.wasm)?;
        for factor in [0, 1, 1234, 3333, 9999, 34000, 1000000] {
            let mut call = Call {
                input: input.clone(),
                dynamic_factor: factor,
                ..Default::default()
            };
            call.initial_storage.insert(U256::ZERO, U256::from(7));
            let expected = native.run(&call)?;
            let mut limits = vec![0, 1, 2, 3, 5, 6, 8, 9, 10, 20, 100, 2048, 2050, 100000];
            limits.extend([
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ]);
            limits.sort_unstable();
            limits.dedup();
            for limit in limits {
                call.energy_limit = limit;
                let expected = native.run(&call)?;
                for _ in 0..2 {
                    let actual = vm.run(&call)?;
                    ensure!(expected==actual,"{name}, factor={factor}, limit={limit}\nnative={expected:?}\ncompiled={actual:?}");
                    checks += 1;
                }
            }
        }
        rows.push(json!({"case":name,"regions":compiler::analyze(&code)}));
    }
    let report = json!({"passed":true,"checks":checks,"cases":rows,"cpp_exceptions":false});
    std::fs::write(
        dir.join("validation.json"),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!(
        "{}",
        json!({"passed":true,"checks":checks,"programs":rows.len(),"cpp_exceptions":false})
    );
    Ok(())
}
