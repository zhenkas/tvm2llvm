// SPDX-License-Identifier: LGPL-3.0-or-later
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use secp256k1::{ecdsa::RecoverableSignature, Message, Secp256k1, SecretKey};
use serde_json::{json, Value};
use sha2::{Digest, Sha256};
use std::{
    hint::black_box,
    path::{Path, PathBuf},
    time::Instant,
};
use tvm2llvm::{
    compiler,
    model::Call,
    native::{NativeRunner, CALLER},
    wasm::WasmRunner,
};
fn compile(code: &[u8], dir: &Path, name: &str) -> Result<compiler::Compilation> {
    if std::env::var("TVM_VARIANT").as_deref() == Ok("v3") {
        return tvm2llvm::compiler_v3::compile(code, dir, name);
    }
    if std::env::var("TVM_VARIANT").as_deref() == Ok("v2") {
        let c = tvm2llvm::compiler_v2::compile(code, dir, name)?;
        Ok(compiler::Compilation {
            cpp: c.cpp,
            wasm: c.wasm,
            generation_ms: c.generation_ms,
            clang_ms: c.clang_ms,
            wasm_bytes: c.wasm_bytes,
        })
    } else {
        compiler::compile(code, dir, name)
    }
}
fn input(signature: &str, args: &[U256]) -> Vec<u8> {
    let mut b = keccak256(signature.as_bytes())[..4].to_vec();
    for x in args {
        b.extend(x.to_be_bytes::<32>());
    }
    b
}
fn check(n: &NativeRunner, w: &mut WasmRunner, c: &Call, label: &str) -> Result<()> {
    let a = n.run(c)?;
    let b = w.run(c)?;
    ensure!(a == b, "{label}\nnative={a:?}\nwasm={b:?}");
    Ok(())
}
fn raw_checks(dir: &Path) -> Result<usize> {
    let mut count = 0;
    let ops = [
        1u8, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19, 0x1a, 0x1b, 0x1c, 0x1d,
    ];
    for op in ops {
        // Three input words, opcode, write top word, return. Unused input words are harmless.
        let code = vec![
            0x60, 0x40, 0x35, 0x60, 0x20, 0x35, 0x60, 0, 0x35, op, 0x60, 0, 0x52, 0x60, 0x20, 0x60,
            0, 0xf3,
        ];
        let a = compile(&code, dir, &format!("op-{op:02x}"))?;
        let n = NativeRunner::new(&code);
        let mut w = WasmRunner::new(&a.wasm)?;
        let values = [
            U256::ZERO,
            U256::from(1),
            U256::from(31),
            U256::from(255),
            U256::MAX,
            U256::from(1) << 255,
            U256::from_limbs([
                0x9876543210abcdef,
                0x76543210fedcba98,
                0x3141592653589793,
                0x123456789abcdef0,
            ]),
        ];
        for (i, x) in values.iter().enumerate() {
            for y in &values {
                let mut c = Call::default();
                for v in [*x, *y, values[(i + 2) % values.len()]] {
                    c.input.extend(v.to_be_bytes::<32>());
                }
                check(&n, &mut w, &c, &format!("opcode {op:02x} {x} {y}"))?;
                count += 1;
            }
        }
    }
    let samples = [
        ("return", "600260030160005260206000f3"),
        ("store", "602a60005560005460005260206000f3"),
        ("rollback", "602a60005560006000fd"),
        ("badjump", "600156"),
        ("false_jump", "600060015700"),
        ("underflow", "01"),
        ("invalid", "fe"),
        ("memory-oog", "6300300000600052"),
        ("returndata-oob", "6001600060003e"),
        ("store-then-oog", "602a6000556300300000600052"),
        ("log-then-oog", "60006000a06300300000600052"),
        ("truncated_push", "7f01"),
        ("address", "3060005260206000f3"),
        ("caller", "3360005260206000f3"),
        ("origin", "3260005260206000f3"),
        ("copy_hash", "3660006000373660002060005260206000f3"),
        ("log", "6001600260005260206000a100"),
        ("gas", "5a60005260206000f3"),
    ];
    for (name, h) in samples {
        let code = hex::decode(h)?;
        let a = compile(&code, dir, name)?;
        let n = NativeRunner::new(&code);
        let mut w = WasmRunner::new(&a.wasm)?;
        for limit in [0, 1, 10, 100, 1000, 100000] {
            for factor in [0, 1234, 34000] {
                let mut c = Call {
                    energy_limit: limit,
                    dynamic_factor: factor,
                    ..Default::default()
                };
                c.input = (0..128).collect();
                c.initial_storage.insert(U256::ZERO, U256::from(7));
                check(&n, &mut w, &c, name)?;
                count += 1;
            }
        }
    }
    Ok(count)
}
fn median(v: &mut [f64]) -> f64 {
    v.sort_by(f64::total_cmp);
    v[v.len() / 2]
}
fn signature_fixture() -> Result<(Message, RecoverableSignature, [u8; 65])> {
    let secp = Secp256k1::new();
    let key = SecretKey::from_slice(&[7; 32])?;
    let msg = Message::from_digest(Sha256::digest(b"tvm2llvm synthetic transaction").into());
    let sig = secp.sign_ecdsa_recoverable(&msg, &key);
    let public = secp.recover_ecdsa(&msg, &sig)?.serialize_uncompressed();
    Ok((msg, sig, public))
}
fn main() -> Result<()> {
    let mode = std::env::args().nth(1).unwrap_or("check".into());
    let mut raw_count = 0;
    let dir = PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts".into()));
    std::fs::create_dir_all(&dir)?;
    if mode == "check" {
        raw_count = raw_checks(&dir)?;
        println!("raw differential checks passed: {raw_count}");
    }
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Bench.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let compiled = compile(&code, &dir, "Bench")?;
    let n = NativeRunner::new(&code);
    let mut w = WasmRunner::new(&compiled.wasm)?;
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
    let mut passed = 0;
    for (name, c) in &cases {
        let expected = n.run(c)?;
        if name == "token-transfer" {
            ensure!(
                expected.status == "success" && expected.logs.len() == 1,
                "transfer fixture must exercise storage and LOG"
            );
        }
        for factor in [0, 1234, 34000] {
            let mut c = c.clone();
            c.dynamic_factor = factor;
            check(&n, &mut w, &c, name)?;
            passed += 1;
            let energy = n.run(&c)?.energy_used;
            for limit in [energy.saturating_sub(1), energy, energy + 1] {
                let mut c = c.clone();
                c.energy_limit = limit;
                check(&n, &mut w, &c, &format!("{name} energy boundary"))?;
                passed += 1;
            }
        }
    }
    println!("Solidity differential checks passed: {passed}");
    if std::env::var("TVM_VARIANT").as_deref() == Ok("v2") {
        ensure!(
            w.caught_exceptions()?.is_some_and(|n| n > 0),
            "v2 failing cases must actually cross the C++ catch boundary"
        );
    }

    if mode != "bench" {
        std::fs::write(
            dir.join("validation.json"),
            serde_json::to_string_pretty(
                &json!({"raw_checks":raw_count,"solidity_checks":passed,"passed":true}),
            )?,
        )?;
        return Ok(());
    }
    let mut rows = vec![];
    let secp = Secp256k1::new();
    let (msg, sig, public) = signature_fixture()?;
    for (name, c) in &cases {
        if name == "gas-probe" {
            continue;
        }
        for _ in 0..20 {
            black_box(n.run(c)?);
            black_box(w.run(c)?);
        }
        let t = Instant::now();
        for _ in 0..20 {
            black_box(n.run(c)?);
        }
        let one = t.elapsed().as_secs_f64() / 20.;
        let iterations = (0.025 / one.max(1e-9)).clamp(10., 10000.) as usize;
        let mut nt = vec![];
        let mut wt = vec![];
        for round in 0..9 {
            for which in if round % 2 == 0 { [0, 1] } else { [1, 0] } {
                let t = Instant::now();
                for _ in 0..iterations {
                    if which == 0 {
                        black_box(n.run(black_box(c))?);
                    } else {
                        black_box(w.run(black_box(c))?);
                    }
                }
                let us = t.elapsed().as_secs_f64() * 1e6 / iterations as f64;
                if which == 0 {
                    nt.push(us);
                } else {
                    wt.push(us);
                }
            }
        }
        let native_us = median(&mut nt);
        let wasm_us = median(&mut wt);
        rows.push(json!({"case":name,"iterations_per_sample":iterations,"samples":9,"native_us":native_us,"wasm_us":wasm_us,"speedup_native_over_wasm":native_us/wasm_us,"native_samples_us":nt,"wasm_samples_us":wt,"energy":n.run(c)?.energy_used}));
    }
    let mut sig_times = vec![];
    for _ in 0..9 {
        let t = Instant::now();
        for _ in 0..1000 {
            let p = secp
                .recover_ecdsa(black_box(&msg), black_box(&sig))?
                .serialize_uncompressed();
            ensure!(p == public);
            black_box(keccak256(&p[1..]));
        }
        sig_times.push(t.elapsed().as_secs_f64() * 1000.);
    }
    let signature_us = median(&mut sig_times);
    // Signed-call comparison uses the same outer transaction recovery work for both backends.
    let signed = &cases[3].1;
    let mut sn = vec![];
    let mut sw = vec![];
    for round in 0..9 {
        for which in if round % 2 == 0 { [0, 1] } else { [1, 0] } {
            let t = Instant::now();
            for _ in 0..200 {
                let p = secp
                    .recover_ecdsa(black_box(&msg), black_box(&sig))?
                    .serialize_uncompressed();
                ensure!(p == public);
                black_box(keccak256(&p[1..]));
                if which == 0 {
                    black_box(n.run(signed)?);
                } else {
                    black_box(w.run(signed)?);
                }
            }
            let us = t.elapsed().as_secs_f64() * 1e6 / 200.;
            if which == 0 {
                sn.push(us);
            } else {
                sw.push(us);
            }
        }
    }
    let signed_native_us = median(&mut sn);
    let signed_wasm_us = median(&mut sw);
    let report = json!({"schema":1,"compiler":compiled,"wasmtime_jit_and_instantiation_ms":w.jit_ms,"solidity_checks":passed,"signature_recovery_us":signature_us,"signed_arithmetic_100":{"native_us":signed_native_us,"wasm_us":signed_wasm_us,"speedup_native_over_wasm":signed_native_us/signed_wasm_us},"cases":rows});
    let path = dir.join("report.json");
    std::fs::write(&path, serde_json::to_string_pretty(&report)?)?;
    println!("{}", serde_json::to_string_pretty(&report)?);
    Ok(())
}
