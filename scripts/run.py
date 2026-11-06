#!/usr/bin/env python3
"""Build the contract compiler and run checks, examples or warmed benchmarks."""
from pathlib import Path
import os
import shutil
import subprocess
import sys
import tomllib

root = Path(__file__).resolve().parents[1]
os.chdir(root)
env = os.environ.copy()
mode = sys.argv[1] if len(sys.argv) > 1 else "check"
if mode not in ("check", "bench", "lookups", "boundaries", "hash", "wasm-hash"):
    raise SystemExit("usage: python3 scripts/run.py [check|bench|lookups|boundaries|hash|wasm-hash]")
if env.get("TVM_WASI_SDK") and not env.get("TVM_CLANG"):
    env["TVM_CLANG"] = str(Path(env["TVM_WASI_SDK"]) / "bin" / "clang++")
if "TVM_WASM_LD" not in env and not shutil.which("wasm-ld"):
    sysroot = Path(subprocess.check_output(["rustc", "--print", "sysroot"], text=True).strip())
    host = next(line.split(": ", 1)[1] for line in
                subprocess.check_output(["rustc", "-vV"], text=True).splitlines()
                if line.startswith("host: "))
    candidate = sysroot / "lib" / "rustlib" / host / "bin" / "rust-lld"
    if not candidate.exists():
        raise SystemExit("Install lld or set TVM_WASM_LD to rust-lld/wasm-ld")
    env["TVM_WASM_LD"] = str(candidate)
if mode == "wasm-hash":
    # Keep the crypto implementation and every transitive source version identical.
    native_lock = tomllib.loads((root / "Cargo.lock").read_text())["package"]
    wasm_lock = tomllib.loads((root / "benchmarks/wasm-keccak/Cargo.lock").read_text())["package"]
    native_sources = {(p["name"], p["version"], p.get("checksum")) for p in native_lock}
    if any((p["name"], p["version"], p.get("checksum")) not in native_sources
           for p in wasm_lock if "source" in p):
        raise SystemExit("Refresh the diagnostic Keccak lock to match the root Cargo.lock")
    target = Path(env.get("CARGO_TARGET_DIR", root / "target")).resolve()
    env["CARGO_TARGET_DIR"] = str(target)
    subprocess.run(["cargo", "build", "--release", "--locked", "--target",
                    "wasm32-unknown-unknown", "--manifest-path",
                    "benchmarks/wasm-keccak/Cargo.toml"], env=env, check=True)
    env["TVM_WASM_KECCAK_LIB"] = str(target / "wasm32-unknown-unknown" /
                                   "release/libtvm2llvm_wasm_keccak.a")
if mode in ("lookups", "boundaries", "hash", "wasm-hash"):
    example = {"lookups": "inspect-lookups", "boundaries": "check-boundaries", "hash": "hash-costs", "wasm-hash": "wasm-keccak"}[mode]
    if mode == "wasm-hash":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "wasm-keccak"))
    if mode == "hash":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "hash-costs"))
    if mode == "boundaries":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "boundaries"))
    subprocess.run(["cargo", "run", "--release", "--locked", "--example", example], env=env, check=True)
else:
    subprocess.run(["cargo", "build", "--release", "--locked"], env=env, check=True)
    target = Path(env.get("CARGO_TARGET_DIR", "target"))
    subprocess.run([str((target / "release" / "tvm2llvm").resolve()), mode], env=env, check=True)
