// SPDX-License-Identifier: LGPL-3.0-or-later
//! Place the same RustCrypto Keccak inside Wasm, with a separate noinline entry.
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
use wasmtime::{Engine, Module};

const EXPORTS: &[&str] = &[
    "execute",
    "input_ptr",
    "output_ptr",
    "output_len",
    "remaining_energy",
];
fn command(c: &mut Command) -> Result<()> {
    let p = c.output()?;
    ensure!(
        p.status.success(),
        "tool failed: {}",
        String::from_utf8_lossy(&p.stderr)
    );
    Ok(())
}
fn link(object: &Path, archive: Option<&Path>, dest: &Path) -> Result<()> {
    let ld = std::env::var("TVM_WASM_LD").unwrap_or("wasm-ld".into());
    let mut c = Command::new(&ld);
    if ld.contains("rust-lld") {
        c.args(["-flavor", "wasm"]);
    }
    c.arg(object);
    if let Some(a) = archive {
        c.arg(a).arg("--export=h_keccak");
    }
    c.args([
        "--no-entry",
        "--export-memory",
        "--initial-memory=4194304",
        "--max-memory=4194304",
        "-z",
        "stack-size=262144",
    ]);
    for name in EXPORTS {
        c.arg(format!("--export={name}"));
    }
    command(c.arg("-o").arg(dest))?;
    let engine = Engine::default();
    let module = Module::from_file(&engine, dest)?;
    let has_import = module
        .imports()
        .any(|i| i.module() == "host" && i.name() == "keccak");
    ensure!(
        has_import == archive.is_none(),
        "hash placement does not match module imports"
    );
    if archive.is_some() {
        ensure!(
            module.exports().any(|e| e.name() == "h_keccak"),
            "missing separate Keccak function"
        );
    }
    Ok(())
}
fn pair(code: &[u8], out: &Path, name: &str, archive: &Path) -> Result<(PathBuf, PathBuf)> {
    let host = compiler::compile(code, out, name)?;
    let local = out.join(format!("{name}-local.wasm"));
    link(&out.join(format!("{name}.o")), Some(archive), &local)?;
    Ok((host.wasm, local))
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
fn timed(
    name: &str,
    c: &Call,
    native: &NativeRunner,
    host: &mut WasmRunner,
    local: &mut WasmRunner,
) -> Result<Value> {
    let expected = native.run(c)?;
    ensure!(
        expected == host.run(c)? && expected == local.run(c)?,
        "{name}: differential mismatch"
    );
    for _ in 0..20 {
        black_box(native.run(c)?);
        black_box(host.run(c)?);
        black_box(local.run(c)?);
    }
    let t = Instant::now();
    for _ in 0..20 {
        black_box(native.run(c)?);
    }
    let it = (0.020 / (t.elapsed().as_secs_f64() / 20.).max(1e-9)).clamp(20., 10000.) as usize;
    let mut samples = [vec![], vec![], vec![]];
    for round in 0..9 {
        for j in 0..3 {
            let b = (round + j) % 3;
            let t = Instant::now();
            for _ in 0..it {
                match b {
                    0 => {
                        black_box(native.run(black_box(c))?);
                    }
                    1 => {
                        black_box(host.run(black_box(c))?);
                    }
                    _ => {
                        black_box(local.run(black_box(c))?);
                    }
                }
            }
            samples[b].push(t.elapsed().as_secs_f64() * 1e6 / it as f64);
        }
    }
    Ok(json!({"case":name,"iterations":it,
        "native_us":median(&samples[0]),"host_import_us":median(&samples[1]),"wasm_local_us":median(&samples[2]),
        "samples_us":{"native":samples[0],"host_import":samples[1],"wasm_local":samples[2]}}))
}
fn main() -> Result<()> {
    let out =
        PathBuf::from(std::env::var("TVM_ARTIFACTS").unwrap_or("artifacts/wasm-keccak".into()));
    std::fs::create_dir_all(&out)?;
    let archive = PathBuf::from(std::env::var("TVM_WASM_KECCAK_LIB")?);
    let source = include_str!("../benchmarks/host-hash.cpp").replace("host_keccak", "h_keccak");
    std::fs::write(out.join("kernel.cpp"), source)?;
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
            .arg(out.join("kernel.cpp"))
            .arg("-o")
            .arg(out.join("kernel.o")),
    )?;
    let host_path = out.join("kernel-host.wasm");
    let local_path = out.join("kernel-local.wasm");
    link(&out.join("kernel.o"), None, &host_path)?;
    link(&out.join("kernel.o"), Some(&archive), &local_path)?;
    let mut host = WasmRunner::new(&host_path)?;
    let mut local = WasmRunner::new(&local_path)?;
    let count = 100000u32;
    let call = Call {
        input: count.to_le_bytes().to_vec(),
        ..Default::default()
    };
    let expected = hex::encode(U256::from_be_bytes(keccak256([0u8; 64]).0).to_le_bytes::<32>());
    ensure!(
        host.run(&call)?.output == expected && local.run(&call)?.output == expected,
        "kernel hash mismatch"
    );
    for _ in 0..3 {
        black_box(host.run(&call)?);
        black_box(local.run(&call)?);
    }
    let payload = [0u8; 64];
    let mut micro = [vec![], vec![], vec![]];
    for round in 0..9 {
        for k in 0..3 {
            let b = (round + k) % 3;
            let t = Instant::now();
            match b {
                0 => {
                    for _ in 0..count {
                        black_box(keccak256(black_box(&payload)));
                    }
                }
                1 => {
                    black_box(host.run(&call)?);
                }
                _ => {
                    black_box(local.run(&call)?);
                }
            }
            micro[b].push(t.elapsed().as_secs_f64() * 1e9 / f64::from(count));
        }
    }
    let code = hex::decode("3660006000373660002060005260206000f3")?;
    let (hp, lp) = pair(&code, &out, "payload", &archive)?;
    let native = NativeRunner::new(&code);
    let mut host = WasmRunner::new(&hp)?;
    let mut local = WasmRunner::new(&lp)?;
    let mut checks = 0u64;
    // Fixed legacy Keccak vectors distinguish 0x01 padding from NIST SHA3's 0x06.
    for (bytes, expected) in [
        (
            b"".as_slice(),
            "c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470",
        ),
        (
            b"abc".as_slice(),
            "4e03657aea45a94fc7d47ba826c8d667c0d1e6e33a64a036ec44f58fa12d6c45",
        ),
    ] {
        let c = Call {
            input: bytes.to_vec(),
            ..Default::default()
        };
        ensure!(
            host.run(&c)?.output == expected && local.run(&c)?.output == expected,
            "known vector"
        );
        checks += 1;
    }
    // Include both rate boundaries, unaligned lengths, multiple blocks, all byte values.
    for size in [
        0, 1, 31, 32, 33, 64, 135, 136, 137, 271, 272, 273, 1024, 65536,
    ] {
        for factor in [0, 1234, 34000] {
            let mut c = Call {
                input: (0..size).map(|i| (i * 17) as u8).collect(),
                dynamic_factor: factor,
                ..Default::default()
            };
            let expected = native.run(&c)?;
            ensure!(
                expected.output == hex::encode(keccak256(&c.input)),
                "payload digest"
            );
            ensure!(
                expected == host.run(&c)? && expected == local.run(&c)?,
                "payload {size}"
            );
            checks += 1;
            for limit in [
                expected.energy_used.saturating_sub(1),
                expected.energy_used,
                expected.energy_used + 1,
            ] {
                c.energy_limit = limit;
                let expected = native.run(&c)?;
                ensure!(
                    expected == host.run(&c)? && expected == local.run(&c)?,
                    "Energy boundary {size}"
                );
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
        let (hp, lp) = pair(&code, &out, &format!("raw-{count}"), &archive)?;
        rows.push(timed(
            &format!("raw-keccak64-{count}"),
            &Call::default(),
            &NativeRunner::new(&code),
            &mut WasmRunner::new(&hp)?,
            &mut WasmRunner::new(&lp)?,
        )?);
    }
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Bench.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let (hp, lp) = pair(&code, &out, "Bench", &archive)?;
    let n = NativeRunner::new(&code);
    let mut h = WasmRunner::new(&hp)?;
    let mut l = WasmRunner::new(&lp)?;
    for count in [0, 1, 10, 100, 1000] {
        rows.push(timed(
            &format!("solidity-hash-{count}"),
            &Call {
                input: input(count),
                ..Default::default()
            },
            &n,
            &mut h,
            &mut l,
        )?);
    }
    // Include rate-boundary and long-input timings, not only the 64-byte loop.
    for size in [1, 135, 136, 137, 1024, 65536] {
        rows.push(timed(
            &format!("payload-{size}"),
            &Call {
                input: (0..size).map(|i| (i * 17) as u8).collect(),
                ..Default::default()
            },
            &native,
            &mut host,
            &mut local,
        )?);
    }
    let mut artifacts = serde_json::Map::new();
    for file in [
        "kernel-host.wasm",
        "kernel-local.wasm",
        "Bench.wasm",
        "Bench-local.wasm",
    ] {
        let bytes = std::fs::read(out.join(file))?;
        artifacts.insert(
            file.into(),
            json!({"bytes":bytes.len(),"sha256":hex::encode(Sha256::digest(bytes))}),
        );
    }
    let cpu = std::fs::read_to_string("/proc/cpuinfo")?
        .lines()
        .find(|s| s.starts_with("model name"))
        .unwrap_or("")
        .to_owned();
    let report = json!({"schema":1,"passed":true,"cpu":cpu,"checks":checks,
        "sha3":"0.11.0","keccak":"0.2.2","wrapper_inline":"never","artifacts":artifacts,
        "micro_iterations":count,
        "micro_ns":{"native_rust":median(&micro[0]),"host_import":median(&micro[1]),"wasm_local":median(&micro[2])},
        "micro_samples_ns":{"native_rust":micro[0],"host_import":micro[1],"wasm_local":micro[2]},
        "cases":rows,
        "limits":"Same source and locked dependencies, LLVM native vs wasm32 plus Cranelift JIT. Wasm-local wrapper is Rust inline(never), separately linked, no host.keccak import. No database or compilation/JIT in timings."});
    std::fs::write(
        out.join("report.json"),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!(
        "{}",
        json!({"passed":true,"checks":checks,"micro_ns":report["micro_ns"],"cases":report["cases"]})
    );
    Ok(())
}
