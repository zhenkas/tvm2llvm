// SPDX-License-Identifier: LGPL-3.0-or-later
//! No compiler needed on the receiving host: run the same ELF and Wasm modules.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use secp256k1::{Message, Secp256k1, SecretKey};
use serde_json::{json, Value};
use sha2::{Digest, Sha256};
use std::{hint::black_box, path::PathBuf, time::Instant};
use tvm2llvm::{
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
    let mut args = std::env::args().skip(1);
    let bundle = PathBuf::from(args.next().unwrap_or(".".into()));
    let output = PathBuf::from(args.next().unwrap_or("host-report.json".into()));
    let offset: usize = args.next().unwrap_or("0".into()).parse()?;
    let fixture: Value = serde_json::from_slice(&std::fs::read(bundle.join("Bench.json"))?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let native = NativeRunner::new(&code);
    let mut a = WasmRunner::new(&bundle.join("v1.wasm"))?;
    let mut b = WasmRunner::new(&bundle.join("v3.wasm"))?;
    ensure!(
        a.caught_exceptions()?.is_none() && b.caught_exceptions()?.is_none(),
        "v1/v3 must not use exception v2"
    );
    let cases = cases()?;
    let mut checks = 0;
    for (name, c) in &cases {
        for factor in [0, 1234, 34000] {
            let mut c = c.clone();
            c.dynamic_factor = factor;
            let expected = native.run(&c)?;
            ensure!(
                expected == a.run(&c)? && expected == b.run(&c)?,
                "{name}: state/energy mismatch"
            );
            checks += 2;
            for limit in [
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ] {
                c.energy_limit = limit;
                let expected = native.run(&c)?;
                ensure!(
                    expected == a.run(&c)? && expected == b.run(&c)?,
                    "{name}: energy boundary mismatch"
                );
                checks += 2;
            }
        }
    }
    let secp = Secp256k1::new();
    // Public synthetic fixture; never an account key or a broadcast transaction.
    let key = SecretKey::from_slice(&[7; 32])?;
    let raw = b"tvm2llvm synthetic transaction";
    let message = Message::from_digest(Sha256::digest(raw).into());
    let sig = secp.sign_ecdsa_recoverable(&message, &key);
    let expected = secp.recover_ecdsa(&message, &sig)?.serialize_uncompressed();
    let verify = || -> Result<()> {
        let message = Message::from_digest(Sha256::digest(black_box(raw)).into());
        let public = secp
            .recover_ecdsa(black_box(&message), black_box(&sig))?
            .serialize_uncompressed();
        ensure!(public == expected, "outer signature changed sender");
        black_box(keccak256(&public[1..]));
        Ok(())
    };
    let mut rows = vec![];
    // Add the same outer signature work before each backend, using arithmetic-100.
    let mut workload: Vec<(String, Call, bool)> = cases
        .iter()
        .map(|(n, c)| (n.clone(), c.clone(), false))
        .collect();
    workload.push(("signed-arithmetic-100".into(), cases[3].1.clone(), true));
    for (name, c, signed) in &workload {
        for _ in 0..20 {
            if *signed {
                verify()?;
            }
            black_box(native.run(c)?);
            if *signed {
                verify()?;
            }
            black_box(a.run(c)?);
            if *signed {
                verify()?;
            }
            black_box(b.run(c)?);
        }
        let t = Instant::now();
        for _ in 0..20 {
            if *signed {
                verify()?;
            }
            black_box(native.run(c)?);
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
                let which = (round + k + offset) % 3;
                let t = Instant::now();
                for _ in 0..iterations {
                    if *signed {
                        verify()?;
                    }
                    match which {
                        0 => {
                            black_box(native.run(black_box(c))?);
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
        let n = median(&samples[0]);
        let v1 = median(&samples[1]);
        let v3 = median(&samples[2]);
        rows.push(json!({"case":name,"signed":signed,"iterations_per_sample":iterations,"native_us":n,"v1_us":v1,"v3_us":v3,
   "v3_speedup_over_v1":v1/v3,"v3_speedup_over_native":n/v3,"samples_us":{"native":samples[0],"v1":samples[1],"v3":samples[2]}}));
    }
    let mut signatures = vec![];
    for _ in 0..9 {
        let t = Instant::now();
        for _ in 0..1000 {
            verify()?;
        }
        signatures.push(t.elapsed().as_secs_f64() * 1000.);
    }
    let cpu = std::fs::read_to_string("/proc/cpuinfo")
        .unwrap_or_default()
        .lines()
        .find(|s| s.starts_with("model name"))
        .unwrap_or("")
        .split_once(':')
        .map(|(_, v)| v.trim().to_owned());
    let report = json!({"schema":1,"checks":checks,"passed":true,"cpu":cpu,"round_offset":offset,
  "engine":"Wasmtime38.0.4/Cranelift; same executable on both hosts, JIT uses each host CPU",
  "jit_and_instantiation_ms":{"v1":a.jit_ms,"v3":b.jit_ms},
  "signature_us":median(&signatures),"signature_samples_us":signatures,
  "signature_path":"outer host SHA256, secp256k1 recovery, expected-public-key check and Keccak address hash; no TVM precompile",
  "cases":rows,"limits":"single frame, in-memory Host; compilation and JIT excluded from warmed calls"});
    if let Some(parent) = output.parent() {
        std::fs::create_dir_all(parent)?;
    }
    std::fs::write(&output, serde_json::to_string_pretty(&report)? + "\n")?;
    println!(
        "{}",
        json!({"passed":true,"checks":checks,"cases":rows.len(),"report":output,"cpu":report["cpu"]})
    );
    Ok(())
}
