#!/usr/bin/env python3
"""Build and run differential checks or the warmed benchmark."""
from pathlib import Path
import os
import shutil
import subprocess
import sys
root = Path(__file__).resolve().parents[1]
os.chdir(root)
env = os.environ.copy()
mode = sys.argv[1] if len(sys.argv) > 1 else "check"
if mode not in ("check", "bench", "lookups"):
    raise SystemExit("usage: python3 scripts/run.py [check|bench|lookups]")
if "TVM_WASM_LD" not in env and not shutil.which("wasm-ld"):
    sysroot = Path(subprocess.check_output(["rustc", "--print", "sysroot"], text=True).strip())
    host = next(line.split(": ", 1)[1] for line in
                subprocess.check_output(["rustc", "-vV"], text=True).splitlines()
                if line.startswith("host: "))
    candidate = sysroot / "lib" / "rustlib" / host / "bin" / "rust-lld"
    if not candidate.exists():
        raise SystemExit("Install lld or set TVM_WASM_LD to rust-lld/wasm-ld")
    env["TVM_WASM_LD"] = str(candidate)
if mode == "lookups":
    subprocess.run(["cargo", "run", "--release", "--locked", "--example", "inspect-lookups"], env=env, check=True)
    raise SystemExit(0)
subprocess.run(["cargo", "build", "--release", "--locked"], env=env, check=True)
target = Path(env.get("CARGO_TARGET_DIR", "target"))
subprocess.run([str((target / "release" / "tvm2llvm").resolve()), mode],
               env=env, check=True)
