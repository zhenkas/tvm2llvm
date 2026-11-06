// SPDX-License-Identifier: LGPL-3.0-or-later
//! Same-host comparison of v1, exception v2, and inline grouped-reservation v3.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use serde_json::{json, Value};
use std::{hint::black_box, path::PathBuf, time::Instant};
use tvm2llvm::{
    compiler, compiler_v2, compiler_v3,
    model::Call,
    native::{NativeRunner, CALLER},
    wasm::WasmRunner,
};
fn input(signature: &str, args: &[U256]) -> Vec<u8> {
    let mut b = keccak256(signature.as_bytes())[..4].to_vec();
    for x in args {
        b.extend(x.to_be_bytes::<32>());
    }
    b
}
fn cases() -> Result<Vec<(String, Call)>> {
    let mut cases = vec![];
    for loops in [0, 1, 10, 100, 1000] {
        cases.push((
            format!("arithmetic-{loops}"),
            Call {
                input: input(
                    "arithmetic(uint256,uint256)",
                    &[U256::from(loops), U256::MAX],
                ),
                ..Default::default()
            },
        ));
    }
    for loops in [1, 10, 100] {
        cases.push((
            format!("hash-{loops}"),
            Call {
                input: input(
                    "hashLoop(uint256,bytes32)",
                    &[U256::from(loops), U256::from(123)],
                ),
                ..Default::default()
            },
        ));
    }
    for x in [0, 1, 7] {
        cases.push((
            format!("branch-{x}"),
            Call {
                input: input("fail(uint256)", &[U256::from(x)]),
                ..Default::default()
            },
        ));
    }
    cases.push((
        "storage-seed".into(),
        Call {
            input: input("seed(uint256)", &[U256::from(1000)]),
            ..Default::default()
        },
    ));
    let mut transfer = Call {
        input: input(
            "transfer(address,uint256)",
            &[U256::from(3), U256::from(17)],
        ),
        ..Default::default()
    };
    let mut slot = [0u8; 64];
    slot[12..32].copy_from_slice(CALLER.as_slice());
    transfer
        .initial_storage
        .insert(U256::from_be_bytes(keccak256(slot).0), U256::from(1000));
    cases.push(("token-transfer".into(), transfer));
    cases.push((
        "gas-probe".into(),
        Call {
            input: input("gasProbe(uint256)", &[U256::from(100)]),
            ..Default::default()
        },
    ));
    cases.push((
        "arithmetic-100-dynamic-energy".into(),
        Call {
            input: input("arithmetic(uint256,uint256)", &[U256::from(100), U256::MAX]),
            dynamic_factor: 34000,
            ..Default::default()
        },
    ));

    let mut failure = cases[3].1.clone();
    failure.energy_limit = 1;
    cases.push(("out-of-energy-exception".into(), failure));
    Ok(cases)
}

fn median(v: &[f64]) -> f64 {
    let mut s = v.to_vec();
    s.sort_by(f64::total_cmp);
    s[s.len() / 2]
}
fn main() -> Result<()> {
    let dir =
        PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts/v3-comparison".into()));
    std::fs::create_dir_all(&dir)?;
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Bench.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let mut compilations = vec![];
    let order = [["v1", "v2", "v3"], ["v2", "v3", "v1"], ["v3", "v1", "v2"]];
    for (round, variants) in order.into_iter().enumerate() {
        for variant in variants {
            let d = dir.join(format!("{variant}-{round}"));
            let compilation = match variant {
                "v1" => serde_json::to_value(compiler::compile(&code, &d, "Bench")?)?,
                "v2" => serde_json::to_value(compiler_v2::compile(&code, &d, "Bench")?)?,
                _ => serde_json::to_value(compiler_v3::compile(&code, &d, "Bench")?)?,
            };
            compilations.push(json!({"round":round,"variant":variant,"compilation":compilation}));
        }
    }
    let native = NativeRunner::new(&code);
    let mut a = WasmRunner::new(&dir.join("v1-2/Bench.wasm"))?;
    let mut b = WasmRunner::new(&dir.join("v2-2/Bench.wasm"))?;
    let mut c = WasmRunner::new(&dir.join("v3-2/Bench.wasm"))?;
    ensure!(
        a.caught_exceptions()?.is_none() && c.caught_exceptions()?.is_none(),
        "v1/v3 have no C++ catch runtime"
    );
    ensure!(
        b.caught_exceptions()? == Some(0),
        "v2 must expose real catch instrumentation"
    );
    let cases = cases()?;
    let mut checks = 0;
    for (name, call) in &cases {
        for factor in [0, 1234, 34000] {
            let mut call = call.clone();
            call.dynamic_factor = factor;
            let expected = native.run(&call)?;
            ensure!(
                expected == a.run(&call)?
                    && expected == b.run(&call)?
                    && expected == c.run(&call)?,
                "{name}: execution mismatch"
            );
            checks += 3;
            for limit in [
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ] {
                call.energy_limit = limit;
                let expected = native.run(&call)?;
                ensure!(
                    expected == a.run(&call)?
                        && expected == b.run(&call)?
                        && expected == c.run(&call)?,
                    "{name}: energy boundary mismatch"
                );
                checks += 3;
            }
        }
    }
    ensure!(
        b.caught_exceptions()?.unwrap() > 0,
        "v2 errors must enter its C++ catch"
    );
    let mut rows = vec![];
    for (name, call) in &cases {
        for _ in 0..20 {
            black_box(native.run(call)?);
            black_box(a.run(call)?);
            black_box(b.run(call)?);
            black_box(c.run(call)?);
        }
        let t = Instant::now();
        for _ in 0..20 {
            black_box(native.run(call)?);
        }
        let one = t.elapsed().as_secs_f64() / 20.;
        let iterations = (0.025 / one.max(1e-9)).clamp(
            10.,
            if name == "out-of-energy-exception" {
                200.
            } else {
                10000.
            },
        ) as usize;
        let mut samples = [vec![], vec![], vec![], vec![]];
        for round in 0..9 {
            for k in 0..4 {
                let which = (round + k) % 4;
                let t = Instant::now();
                for _ in 0..iterations {
                    match which {
                        0 => {
                            black_box(native.run(black_box(call))?);
                        }
                        1 => {
                            black_box(a.run(black_box(call))?);
                        }
                        2 => {
                            black_box(b.run(black_box(call))?);
                        }
                        _ => {
                            black_box(c.run(black_box(call))?);
                        }
                    }
                }
                samples[which].push(t.elapsed().as_secs_f64() * 1e6 / iterations as f64);
            }
        }
        let n = median(&samples[0]);
        let v1 = median(&samples[1]);
        let v2 = median(&samples[2]);
        let v3 = median(&samples[3]);
        rows.push(json!({"case":name,"iterations_per_sample":iterations,"native_us":n,"v1_us":v1,"v2_us":v2,"v3_us":v3,
   "v3_speedup_over_v1":v1/v3,"v3_speedup_over_v2":v2/v3,"v3_speedup_over_native":n/v3,
   "samples_us":{"native":samples[0],"v1":samples[1],"v2":samples[2],"v3":samples[3]}}));
    }
    let compile_times = |v: &str| -> Vec<f64> {
        compilations
            .iter()
            .filter(|r| r["variant"] == v)
            .map(|r| r["compilation"]["clang_ms"].as_f64().unwrap())
            .collect()
    };
    let plan = compiler_v3::analyze(&code);
    let batched_ops: usize = plan
        .iter()
        .filter(|p| p.batched)
        .map(|p| p.instructions)
        .sum();
    let batched_regions = plan.iter().filter(|p| p.batched).count();
    let clang = std::env::var("TVM_CLANG").unwrap_or("clang++".into());
    let version = std::process::Command::new(&clang)
        .arg("--version")
        .output()?;
    let report = json!({"schema":1,"experiment":"v3 inline static-region reservation, per-op dynamic-factor rounding, exact cold fault checks, no C++ exceptions",
  "checks":checks,"same_clang":std::env::var("TVM_CLANG_V2").unwrap_or(clang.clone())==clang,
  "clang":String::from_utf8_lossy(&version.stdout),"engine":"Wasmtime 38.0.4 / Cranelift; identical EH/gc-drc settings for all modules",
  "compilations":compilations,
  "median_compile_and_link_ms":{"v1":median(&compile_times("v1")),"v2":median(&compile_times("v2")),"v3":median(&compile_times("v3"))},
  "jit_and_instantiation_ms":{"v1":a.jit_ms,"v2":b.jit_ms,"v3":c.jit_ms},
  "region_plan":{"regions":plan.len(),"batched_regions":batched_regions,"batched_instructions":batched_ops,"saved_static_reservations":batched_ops-batched_regions,"plan":plan},
  "v3_exceptions":false,"cases":rows});
    std::fs::write(
        dir.join("comparison.json"),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!("{}", serde_json::to_string_pretty(&report)?);
    Ok(())
}
