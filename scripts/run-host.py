#!/usr/bin/env python3
"""Run a checksum-verified, precompiled native/v1/v3 bundle on Linux."""
from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import time

def digest(path):
    with path.open("rb") as f:
        return hashlib.file_digest(f, "sha256").hexdigest()

def snapshot():
    result = {"at": time.time(), "cpu_count": os.cpu_count(),
              "affinity_count": len(os.sched_getaffinity(0))}
    cpu = Path("/proc/cpuinfo").read_text().splitlines()
    result["cpu"] = next((s.split(":", 1)[1].strip() for s in cpu if s.startswith("model name")), None)
    result["flags"] = next((s.split(":", 1)[1].strip() for s in cpu if s.startswith("flags")), None)
    for name in ("cpu", "memory", "io"):
        try:
            result["pressure_" + name] = Path("/proc/pressure", name).read_text().strip()
        except OSError:
            pass
    try:
        group = Path("/proc/self/cgroup").read_text().strip().split("::", 1)[1]
        base = Path("/sys/fs/cgroup") / group.lstrip("/")
        own = {}
        for name in ("cpu.max", "cpu.stat", "memory.high", "memory.max", "memory.current", "memory.events"):
            try:
                own[name] = (base / name).read_text().strip()
            except OSError:
                pass
        result["own_cgroup"] = own
    except (OSError, IndexError):
        result["own_cgroup"] = {}
    return result

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("bundle", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("--host", required=True)
    ap.add_argument("--passes", type=int, default=3)
    args = ap.parse_args()
    bundle = args.bundle.resolve()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    manifest = json.loads((bundle / "manifest.json").read_text())
    for name, info in manifest["files"].items():
        path = bundle / name
        if path.stat().st_size != info["bytes"] or digest(path) != info["sha256"]:
            raise SystemExit("Bundle mismatch: " + name)
    results = []
    for index in range(args.passes):
        path = output / ("pass-" + str(index + 1) + ".json")
        before = snapshot()
        started = time.monotonic()
        p = subprocess.run([str(bundle / "compare-host"), str(bundle), str(path), str(index)],
                           capture_output=True, text=True)
        after = snapshot()
        result = {"pass": index + 1, "exit": p.returncode,
                  "seconds": time.monotonic() - started, "before": before, "after": after}
        results.append(result)
        (output / "execution.json").write_text(json.dumps(
            {"host": args.host, "manifest": manifest, "passes": results}, indent=2) + "\n")
        print(json.dumps({"host": args.host, "pass": index + 1, "exit": p.returncode,
                          "stdout": p.stdout.strip(), "stderr": p.stderr[-2000:]}), flush=True)
        if p.returncode:
            raise SystemExit(p.returncode)
        report = json.loads(path.read_text())
        if not report["passed"]:
            raise SystemExit("Differential validation did not pass")
    print(json.dumps({"host": args.host, "passes": len(results), "passed": True}), flush=True)

if __name__ == "__main__":
    main()
