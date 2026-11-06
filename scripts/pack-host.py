#!/usr/bin/env python3
"""Package a Linux benchmark executable and the compiled contract module."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tarfile

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("output", type=Path)
    p.add_argument("--binary", type=Path, required=True)
    p.add_argument("--wasm", type=Path, required=True)
    a = p.parse_args()
    root = Path(__file__).resolve().parents[1]
    a.output.mkdir(parents=True, exist_ok=False)
    bundle = a.output / "bundle"
    bundle.mkdir()
    files = {"compare-host": a.binary, "compiled.wasm": a.wasm,
             "Bench.json": root / "fixtures/Bench.json",
             "run-host.py": root / "scripts/run-host.py",
             "SOURCE.json": root / "vendor/SOURCE.json"}
    manifest = {"schema": 1, "baseline_commit": subprocess.check_output(
        ["git", "rev-parse", "HEAD"], cwd=root, text=True).strip(),
        "note": "Record build flags separately; use identical binary bytes on each host.",
        "files": {}}
    for name, source in files.items():
        dest = bundle / name
        shutil.copy2(source, dest)
        manifest["files"][name] = {"bytes": dest.stat().st_size,
                                  "sha256": hashlib.sha256(dest.read_bytes()).hexdigest()}
    (bundle / "compare-host").chmod(0o755)
    (bundle / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    archive = a.output / "benchmark-bundle.tar.gz"
    with tarfile.open(archive, "w:gz") as t:
        t.add(bundle, arcname="bundle")
    print(json.dumps({"archive": str(archive), "bytes": archive.stat().st_size,
                      "sha256": hashlib.sha256(archive.read_bytes()).hexdigest()}))

if __name__ == "__main__":
    main()
