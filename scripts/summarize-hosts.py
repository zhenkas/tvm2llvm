#!/usr/bin/env python3
"""Aggregate repeated cross-host runs; retain every warmed timing sample."""
import argparse
import json
from pathlib import Path
from statistics import median

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("output", type=Path)
    ap.add_argument("runs", nargs="+", type=Path)
    args = ap.parse_args()
    hosts = []
    manifest = None
    for directory in args.runs:
        execution = json.loads((directory / "execution.json").read_text())
        if manifest is None:
            manifest = execution["manifest"]
        if manifest != execution["manifest"]:
            raise SystemExit("Hosts used different bundles")
        passes = execution["passes"]
        if not passes or any(p["exit"] != 0 for p in passes):
            raise SystemExit("Incomplete or unsuccessful run")
        reports = [json.loads((directory / ("pass-" + str(p["pass"]) + ".json")).read_text())
                   for p in passes]
        names = [r["case"] for r in reports[0]["cases"]]
        if any(not r["passed"] or [c["case"] for c in r["cases"]] != names for r in reports):
            raise SystemExit("Validation or workload mismatch")
        rows = []
        for index, name in enumerate(names):
            samples = {backend: [s for report in reports
                                  for s in report["cases"][index]["samples_us"][backend]]
                       for backend in ("native", "compiled")}
            medians = {backend: median(s) for backend, s in samples.items()}
            rows.append({"case": name, "median_us": medians,
                         "speedup_native_over_compiled": medians["native"] / medians["compiled"],
                         "samples_us": samples,
                         "pass_median_us": [{b: r["cases"][index][b + "_us"]
                                             for b in ("native", "compiled")} for r in reports],
                         "iterations_per_sample": [r["cases"][index]["iterations_per_sample"]
                                                   for r in reports]})
        signatures = [s for r in reports for s in r["signature_samples_us"]]
        hosts.append({"host": execution["host"], "cpu": reports[0]["cpu"],
                      "passes": len(reports), "validation_checks": sum(r["checks"] for r in reports),
                      "passed": True, "cases": rows, "signature_median_us": median(signatures),
                      "signature_samples_us": signatures,
                      "jit_and_instantiation_ms": [r["jit_and_instantiation_ms"] for r in reports],
                      "observation": [{"pass": p["pass"], "seconds": p["seconds"],
                                       "before": p["before"], "after": p["after"]} for p in passes]})
    result = {"schema": 1, "bundle_manifest": manifest,
              "method": "Median of 27 batch timings per backend/workload: 3 process passes, 9 rotated-order samples each. Single-frame in-memory Host, same ELF and Wasm bytes on both hosts. Compile/JIT excluded.",
              "worker_limits": {"cpu_percent": 250, "memory_high_bytes": 6442450944,
                                "memory_max_bytes": 8589934592, "tasks_max": 128},
              "hosts": hosts}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    for host in hosts:
        print(host["host"], host["cpu"], "checks", host["validation_checks"])
        for row in host["cases"]:
            print(row["case"], json.dumps(row["median_us"]), "speedup", row["speedup_native_over_compiled"])
        print("signature_us", host["signature_median_us"])

if __name__ == "__main__":
    main()
