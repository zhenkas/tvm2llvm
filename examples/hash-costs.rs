// SPDX-License-Identifier: LGPL-3.0-or-later
//! Hash kernel, actual Wasm import, empty-import control, raw opcode and Solidity costs.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use serde_json::{json, Value};
use sha2::{Digest, Sha256};
use std::{
    hint::black_box,
    path::{Path, PathBuf},
    process::Command,
    time::Instant,
};
use tvm2llvm::{compiler, model::Call, native::NativeRunner, wasm::WasmRunner};
use wasmtime::{Caller, Engine, Linker, Module, Store};
fn command(c: &mut Command) -> Result<()> {
    let p = c.output()?;
    ensure!(
        p.status.success(),
        "tool failed: {}",
        String::from_utf8_lossy(&p.stderr)
    );
    Ok(())
}
fn kernel(source: &str, dir: &Path, name: &str, exports: &[&str]) -> Result<PathBuf> {
    std::fs::create_dir_all(dir)?;
    let cpp = dir.join(format!("{name}.cpp"));
    let obj = dir.join(format!("{name}.o"));
    let wasm = dir.join(format!("{name}.wasm"));
    std::fs::write(&cpp, source)?;
    command(
        Command::new(std::env::var("TVM_CLANG").unwrap_or("clang++".into()))
            .args([
                "--target=wasm32-unknown-unknown",
                "-O3",
                "-std=c++17",
                "-ffreestanding",
                "-fno-builtin",
                "-fno-exceptions",
                "-fno-rtti",
                "-c",
            ])
            .arg(&cpp)
            .arg("-o")
            .arg(&obj),
    )?;
    let ld = std::env::var("TVM_WASM_LD").unwrap_or("wasm-ld".into());
    let mut c = Command::new(&ld);
    if ld.contains("rust-lld") {
        c.args(["-flavor", "wasm"]);
    }
    c.arg(&obj).args([
        "--no-entry",
        "--export-memory",
        "--initial-memory=4194304",
        "--max-memory=4194304",
    ]);
    for name in exports {
        c.arg(format!("--export={name}"));
    }
    command(c.arg("-o").arg(&wasm))?;
    Ok(wasm)
}
fn median(v: &[f64]) -> f64 {
    let mut v = v.to_vec();
    v.sort_by(f64::total_cmp);
    v[v.len() / 2]
}
fn input(n: u64) -> Vec<u8> {
    let mut v = keccak256(b"hashLoop(uint256,bytes32)")[..4].to_vec();
    v.extend(U256::from(n).to_be_bytes::<32>());
    v.extend(U256::from(123).to_be_bytes::<32>());
    v
}
fn timed_case(name: &str, c: &Call, n: &NativeRunner, w: &mut WasmRunner) -> Result<Value> {
    ensure!(n.run(c)? == w.run(c)?, "{name}: outcome/Energy mismatch");
    for _ in 0..20 {
        black_box(n.run(c)?);
        black_box(w.run(c)?);
    }
    let t = Instant::now();
    for _ in 0..20 {
        black_box(n.run(c)?);
    }
    let it = (0.030 / (t.elapsed().as_secs_f64() / 20.).max(1e-9)).clamp(20., 10000.) as usize;
    let mut samples = [vec![], vec![]];
    for round in 0..9 {
        for j in 0..2 {
            let b = (round + j) % 2;
            let t = Instant::now();
            for _ in 0..it {
                if b == 0 {
                    black_box(n.run(black_box(c))?);
                } else {
                    black_box(w.run(black_box(c))?);
                }
            }
            samples[b].push(t.elapsed().as_secs_f64() * 1e6 / it as f64);
        }
    }
    Ok(
        json!({"case":name,"iterations":it,"native_us":median(&samples[0]),"compiled_us":median(&samples[1]),"samples_us":{"native":samples[0],"compiled":samples[1]}}),
    )
}
fn main() -> Result<()> {
    let out =
        PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts/hash-costs".into()));
    std::fs::create_dir_all(&out)?;
    // This kernel uses the actual production WasmRunner hash callback, without TVM work.
    let path = kernel(
        include_str!("../benchmarks/host-hash.cpp"),
        &out,
        "host-hash",
        &[
            "execute",
            "input_ptr",
            "output_ptr",
            "output_len",
            "remaining_energy",
        ],
    )?;
    let mut imported = WasmRunner::new(&path)?;
    let call = Call {
        input: 100000u32.to_le_bytes().to_vec(),
        ..Default::default()
    };
    let expected = hex::encode(U256::from_be_bytes(keccak256([0u8; 64]).0).to_le_bytes::<32>());
    ensure!(
        imported.run(&call)?.output == expected,
        "actual callback hash differs"
    );
    let noop = kernel(
        include_str!("../benchmarks/host-noop.cpp"),
        &out,
        "host-noop",
        &["run"],
    )?;
    let engine = Engine::default();
    let module = Module::from_file(&engine, noop)?;
    let mut linker = Linker::new(&engine);
    linker.func_wrap(
        "host",
        "noop",
        |mut c: Caller<'_, u64>, p: i32, n: i32, v: i32| {
            *c.data_mut() += 1;
            black_box((p, n, v));
        },
    )?;
    let mut store = Store::new(&engine, 0u64);
    let instance = linker.instantiate(&mut store, &module)?;
    let empty = instance.get_typed_func::<i32, ()>(&mut store, "run")?;
    let mut micro = [vec![], vec![], vec![]];
    let payload = [0u8; 64];
    let count = 100000;
    for _ in 0..3 {
        black_box(imported.run(&call)?);
        empty.call(&mut store, count)?;
    }
    for round in 0..9 {
        for k in 0..3 {
            let which = (round + k) % 3;
            let t = Instant::now();
            match which {
                0 => {
                    for _ in 0..count {
                        black_box(keccak256(black_box(&payload)));
                    }
                }
                1 => {
                    black_box(imported.run(&call)?);
                }
                _ => {
                    empty.call(&mut store, count)?;
                }
            }
            micro[which].push(t.elapsed().as_secs_f64() * 1e9 / count as f64);
        }
    }
    ensure!(
        *store.data() == 12 * count as u64,
        "empty import loop call count"
    );
    // A generic copy/hash frame checks borrowed memory across Keccak rate boundaries.
    let code = hex::decode("3660006000373660002060005260206000f3")?;
    let c = compiler::compile(&code, &out, "payload-hash")?;
    let native = NativeRunner::new(&code);
    let mut wasm = WasmRunner::new(&c.wasm)?;
    let mut checks = 0;
    for size in [0, 1, 31, 32, 33, 64, 135, 136, 137, 1024, 65536] {
        for factor in [0, 1234, 34000] {
            let mut c = Call {
                input: (0..size).map(|i| (i * 17) as u8).collect(),
                dynamic_factor: factor,
                ..Default::default()
            };
            let expected = native.run(&c)?;
            ensure!(
                expected.output == hex::encode(keccak256(&c.input)),
                "native hash payload {size}"
            );
            ensure!(expected == wasm.run(&c)?, "payload {size}");
            checks += 1;
            for limit in [
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ] {
                c.energy_limit = limit;
                ensure!(native.run(&c)? == wasm.run(&c)?, "payload boundary {size}");
                checks += 1;
            }
        }
    }
    let mut rows = vec![];
    for count in [1, 100] {
        let mut code = vec![];
        for i in 0..count {
            code.extend([0x60, 64, 0x60, 0, 0x20]);
            if i + 1 < count {
                code.push(0x50);
            }
        }
        code.extend([0x60, 0, 0x52, 0x60, 32, 0x60, 0, 0xf3]);
        let c = compiler::compile(&code, &out, &format!("raw-hash-{count}"))?;
        let n = NativeRunner::new(&code);
        let mut w = WasmRunner::new(&c.wasm)?;
        rows.push(timed_case(
            &format!("raw-keccak64-{count}"),
            &Call::default(),
            &n,
            &mut w,
        )?);
    }
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Bench.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let c = compiler::compile(&code, &out, "Bench")?;
    let n = NativeRunner::new(&code);
    let mut w = WasmRunner::new(&c.wasm)?;
    for count in [0, 1, 10, 100, 1000] {
        rows.push(timed_case(
            &format!("solidity-hash-{count}"),
            &Call {
                input: input(count),
                ..Default::default()
            },
            &n,
            &mut w,
        )?);
    }
    let cpu = std::fs::read_to_string("/proc/cpuinfo")?
        .lines()
        .find(|s| s.starts_with("model name"))
        .unwrap_or("")
        .to_owned();
    let report = json!({"schema":1,"cpu":cpu,"passed":true,"payload_energy_checks":checks,
       "host_bridge_sha256":hex::encode(Sha256::digest(include_bytes!("../src/wasm.rs"))),
       "micro_ns":{"direct_rust_keccak64":median(&micro[0]),"actual_wasm_import_keccak64":median(&micro[1]),"empty_wasm_import":median(&micro[2])},
       "micro_samples_ns":{"direct_rust_keccak64":micro[0],"actual_wasm_import_keccak64":micro[1],"empty_wasm_import":micro[2]},
       "micro_iterations":count,"verified_empty_import_calls":*store.data(),"cases":rows,"limits":"Single host; micro kernels omit TVM resource work; whole-call rows include it. Do not subtract noisy aggregate medians as an exact additive model."});
    // The output path is relative to the diagnostics directory.
    let name = std::env::args().nth(1).unwrap_or("report.json".into());
    std::fs::write(
        out.join(name),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!(
        "{}",
        json!({"passed":true,"micro_ns":report["micro_ns"],"cases":report["cases"],"payload_energy_checks":checks})
    );
    Ok(())
}
