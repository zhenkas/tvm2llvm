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
if mode not in ("check", "bench", "lookups", "check-v2", "compare-v2", "lookups-v2", "check-v3", "compare-v3", "lookups-v3", "boundaries-v3"):
    raise SystemExit("usage: python3 scripts/run.py [check|bench|lookups|check-v2|compare-v2|lookups-v2|check-v3|compare-v3|lookups-v3|boundaries-v3]")
if mode.endswith("v2") or mode == "compare-v3":
    sdk = env.get("TVM_WASI_SDK")
    clang = env.get("TVM_CLANG_V2") or (str(Path(sdk) / "bin" / "clang++") if sdk else None)
    if not clang:
        raise SystemExit("Set TVM_WASI_SDK to wasi-sdk 34 (see experiments/v2/README.md)")
    env["TVM_CLANG_V2"] = clang
    # Both comparison variants must use the identical optimizer.
    env["TVM_CLANG"] = clang
    if mode == "check-v2":
        env["TVM_VARIANT"] = "v2"
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "v2-check"))
    elif mode == "compare-v2":
        env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "v2-comparison"))
if mode == "check-v3":
    env["TVM_VARIANT"] = "v3"
    env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "v3-check"))
elif mode == "boundaries-v3":
    env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "v3-boundaries"))
elif mode == "compare-v3":
    env.setdefault("TVM_ARTIFACTS", str(root / "artifacts" / "v3-comparison"))
if "TVM_WASM_LD" not in env and not shutil.which("wasm-ld"):
    sysroot = Path(subprocess.check_output(["rustc", "--print", "sysroot"], text=True).strip())
    host = next(line.split(": ", 1)[1] for line in
                subprocess.check_output(["rustc", "-vV"], text=True).splitlines()
                if line.startswith("host: "))
    candidate = sysroot / "lib" / "rustlib" / host / "bin" / "rust-lld"
    if not candidate.exists():
        raise SystemExit("Install lld or set TVM_WASM_LD to rust-lld/wasm-ld")
    env["TVM_WASM_LD"] = str(candidate)
if mode in ("lookups","lookups-v2","compare-v2","lookups-v3","compare-v3","boundaries-v3"):
    example = {"lookups":"inspect-lookups","lookups-v2":"inspect-lookups-v2","compare-v2":"compare-v2","lookups-v3":"inspect-lookups-v3","compare-v3":"compare-v3","boundaries-v3":"check-v3-boundaries"}[mode]
    subprocess.run(["cargo", "run", "--release", "--locked", "--example", example], env=env, check=True)
    raise SystemExit(0)
subprocess.run(["cargo", "build", "--release", "--locked"], env=env, check=True)
target = Path(env.get("CARGO_TARGET_DIR", "target"))
subprocess.run([str((target / "release" / "tvm2llvm").resolve()), "check" if mode in ("check-v2","check-v3") else mode],
               env=env, check=True)
