// SPDX-License-Identifier: LGPL-3.0-or-later
//! Same-engine, same-Clang comparison of preserved v1 and experimental exception v2.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use serde_json::{json, Value};
use std::{hint::black_box, path::PathBuf, time::Instant};
use tvm2llvm::{
    compiler, compiler_v2,
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
        PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts/v2-comparison".into()));
    std::fs::create_dir_all(&dir)?;
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Bench.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let mut compile_rows = vec![];
    // Alternate compilation order. All variants use the same Clang executable.
    for round in 0..3 {
        for variant in if round % 2 == 0 {
            ["v1", "v2"]
        } else {
            ["v2", "v1"]
        } {
            let d = dir.join(format!("{variant}-{round}"));
            let c = if variant == "v1" {
                serde_json::to_value(compiler::compile(&code, &d, "Bench")?)?
            } else {
                serde_json::to_value(compiler_v2::compile(&code, &d, "Bench")?)?
            };
            compile_rows.push(json!({"round":round,"variant":variant,"compilation":c}));
        }
    }
    let n = NativeRunner::new(&code);
    let mut a = WasmRunner::new(&dir.join("v1-2/Bench.wasm"))?;
    let mut b = WasmRunner::new(&dir.join("v2-2/Bench.wasm"))?;
    ensure!(
        a.caught_exceptions()?.is_none(),
        "v1 must not use the v2 exception boundary"
    );
    ensure!(
        b.caught_exceptions()? == Some(0),
        "v2 must export its real catch counter"
    );
    let cases = cases()?;
    let mut checks = 0;
    for (name, c) in &cases {
        for factor in [0, 1234, 34000] {
            let mut c = c.clone();
            c.dynamic_factor = factor;
            let expected = n.run(&c)?;
            ensure!(
                expected == a.run(&c)? && expected == b.run(&c)?,
                "{name}: result mismatch"
            );
            checks += 2;
            for limit in [
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ] {
                c.energy_limit = limit;
                let expected = n.run(&c)?;
                ensure!(
                    expected == a.run(&c)? && expected == b.run(&c)?,
                    "{name}: Energy boundary mismatch"
                );
                checks += 2;
            }
        }
    }
    ensure!(
        b.caught_exceptions()?.unwrap() > 0,
        "failing fixtures must actually reach catch"
    );
    // Verify a failed call cannot poison the next successful call on the reused instance.
    let mut overflow = vec![0x5f; 1025];
    overflow.push(0);
    let mut raw_rows = vec![];
    for (name, code) in [
        ("stack-overflow", overflow),
        ("underflow", vec![1]),
        ("out-of-range-memory", hex::decode("6300300000600052")?),
        (
            "storage-rollback",
            hex::decode("602a6000556300300000600052")?,
        ),
        ("log-rollback", hex::decode("60006000a06300300000600052")?),
    ] {
        let refvm = NativeRunner::new(&code);
        let compiled = compiler_v2::compile(&code, &dir.join("errors"), name)?;
        let mut vm = WasmRunner::new(&compiled.wasm)?;
        for factor in [0, 1234, 34000] {
            for limit in [0, 1, 100, 100000] {
                let c = Call {
                    energy_limit: limit,
                    dynamic_factor: factor,
                    ..Default::default()
                };
                let expected = refvm.run(&c)?;
                ensure!(
                    expected == vm.run(&c)?,
                    "{name}: exceptional state mismatch"
                );
                ensure!(expected == vm.run(&c)?, "{name}: repeated call mismatch");
                checks += 2;
            }
        }
        ensure!(
            vm.caught_exceptions()?.unwrap() > 0,
            "{name}: did not throw"
        );
        raw_rows.push(json!({"case":name,"caught":vm.caught_exceptions()?}));
    }
    let mut rows = vec![];
    for (name, c) in &cases {
        for _ in 0..20 {
            black_box(n.run(c)?);
            black_box(a.run(c)?);
            black_box(b.run(c)?);
        }
        let t = Instant::now();
        for _ in 0..20 {
            black_box(n.run(c)?);
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
        let mut samples = [vec![], vec![], vec![]];
        for round in 0..9 {
            for k in 0..3 {
                let which = (k + round) % 3;
                let t = Instant::now();
                for _ in 0..iterations {
                    match which {
                        0 => {
                            black_box(n.run(black_box(c))?);
                        }
                        1 => {
                            black_box(a.run(black_box(c))?);
                        }
                        _ => {
                            black_box(b.run(black_box(c))?);
                        }
                    }
                }
                samples[which].push(t.elapsed().as_secs_f64() * 1e6 / iterations as f64);
            }
        }
        let native = median(&samples[0]);
        let v1 = median(&samples[1]);
        let v2 = median(&samples[2]);
        rows.push(json!({"case":name,"iterations_per_sample":iterations,"native_us":native,"v1_us":v1,"v2_us":v2,"v2_speedup_over_v1":v1/v2,"samples_us":{"native":samples[0],"v1":samples[1],"v2":samples[2]}}));
    }
    let clang = std::env::var("TVM_CLANG").unwrap_or("clang++".into());
    let clang_version = std::process::Command::new(&clang)
        .arg("--version")
        .output()?;
    let compile_ms = |variant: &str| -> Vec<f64> {
        compile_rows
            .iter()
            .filter(|r| r["variant"] == variant)
            .map(|r| r["compilation"]["clang_ms"].as_f64().unwrap())
            .collect()
    };
    let report = json!({"schema":1,"experiment":"v2 noinline reserve<Op> + one C++ try/catch; all other fixture semantics retained",
  "clang":String::from_utf8_lossy(&clang_version.stdout),"same_clang":std::env::var("TVM_CLANG_V2").unwrap_or(clang.clone())==clang,
  "engine":"Wasmtime 38.0.4 / Cranelift, exceptions and gc-drc enabled for BOTH variants",
  "checks":checks,"raw_exception_cases":raw_rows,"caught_exceptions":b.caught_exceptions()?,
  "compilations":compile_rows,"median_compile_and_link_ms":{"v1":median(&compile_ms("v1")),"v2":median(&compile_ms("v2"))},
  "jit_and_instantiation_ms":{"v1":a.jit_ms,"v2":b.jit_ms},"cases":rows,
  "limits":"single frame; warm runs exclude compilation and JIT; v2 includes libc++abi/libunwind; no database/mainnet services"});
    std::fs::write(
        dir.join("comparison.json"),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!("{}", serde_json::to_string_pretty(&report)?);
    Ok(())
}
