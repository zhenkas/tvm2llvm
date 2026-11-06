#!/usr/bin/env python3
"""Build the contract compiler and run checks, examples or warmed benchmarks."""
from pathlib import Path
import os
import shutil
import subprocess
import sys

root = Path(__file__).resolve().parents[1]
os.chdir(root)
env = os.environ.copy()
mode = sys.argv[1] if len(sys.argv) > 1 else "check"
if mode not in ("check", "bench", "lookups", "boundaries", "hash"):
    raise SystemExit("usage: python3 scripts/run.py [check|bench|lookups|boundaries|hash]")
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
if mode in ("lookups", "boundaries", "hash"):
    example = {"lookups": "inspect-lookups", "boundaries": "check-boundaries", "hash": "hash-costs"}[mode]
    if mode == "hash":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "hash-costs"))
    if mode == "boundaries":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "boundaries"))
    subprocess.run(["cargo", "run", "--release", "--locked", "--example", example], env=env, check=True)
else:
    subprocess.run(["cargo", "build", "--release", "--locked"], env=env, check=True)
    target = Path(env.get("CARGO_TARGET_DIR", "target"))
    subprocess.run([str((target / "release" / "tvm2llvm").resolve()), mode], env=env, check=True)
