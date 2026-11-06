# Execution baseline — 2026-11-06

The reference is the TRON-patched Rust TVM interpreter. The compiled backend specializes the same Solidity runtime bytecode into C++, then executes its freestanding Wasm through Wasmtime. There is one compiler/runtime and no backend-version selector.

Median microseconds per warmed call; lower is better. Native / compiled above 1 means compiled is faster. Three process passes give 27 batch samples per backend/workload/host. Compilation and JIT/instantiation are excluded.

## AMD Ryzen 9 9950X 16-Core Processor

| Workload | Native TVM us | Compiled us | Native / compiled |
| --- | ---: | ---: | ---: |
| arithmetic-0 | 0.675 | 0.819 | 0.82x |
| arithmetic-1 | 0.730 | 0.892 | 0.82x |
| arithmetic-10 | 1.287 | 1.239 | 1.04x |
| arithmetic-100 | 6.917 | 4.841 | 1.43x |
| arithmetic-1000 | 62.992 | 39.839 | 1.58x |
| hash-1 | 1.034 | 1.465 | 0.71x |
| hash-10 | 4.403 | 6.222 | 0.71x |
| hash-100 | 37.718 | 53.673 | 0.70x |
| branch-0 | 0.738 | 0.969 | 0.76x |
| branch-1 | 0.666 | 0.743 | 0.90x |
| branch-7 | 0.655 | 0.725 | 0.90x |
| storage-seed | 1.108 | 1.295 | 0.86x |
| token-transfer | 2.947 | 4.004 | 0.74x |
| gas-probe | 5.104 | 3.827 | 1.33x |
| arithmetic-100-dynamic-energy | 10.637 | 4.936 | 2.16x |
| out-of-energy | 0.290 | 0.267 | 1.08x |
| signed-arithmetic-100 | 29.407 | 27.104 | 1.08x |

Outer signature recovery: 22.07 us median.

JIT + instantiation per pass: 83.66 ms, 74.87 ms, 68.06 ms.

Arithmetic-1000 pass medians: native 68.09, 62.99, 58.54 us; compiled 44.71, 39.47, 37.55 us.

## Intel(R) Xeon(R) Silver 4216 CPU @ 2.10GHz

| Workload | Native TVM us | Compiled us | Native / compiled |
| --- | ---: | ---: | ---: |
| arithmetic-0 | 4.041 | 4.064 | 0.99x |
| arithmetic-1 | 3.952 | 3.849 | 1.03x |
| arithmetic-10 | 6.748 | 5.889 | 1.15x |
| arithmetic-100 | 46.086 | 27.679 | 1.67x |
| arithmetic-1000 | 347.541 | 237.249 | 1.46x |
| hash-1 | 6.401 | 6.754 | 0.95x |
| hash-10 | 30.814 | 37.079 | 0.83x |
| hash-100 | 270.182 | 343.245 | 0.79x |
| branch-0 | 6.296 | 6.292 | 1.00x |
| branch-1 | 3.519 | 3.387 | 1.04x |
| branch-7 | 3.422 | 3.413 | 1.00x |
| storage-seed | 6.171 | 7.330 | 0.84x |
| token-transfer | 15.741 | 19.647 | 0.80x |
| gas-probe | 27.559 | 21.274 | 1.30x |
| arithmetic-100-dynamic-energy | 88.993 | 28.154 | 3.16x |
| out-of-energy | 1.185 | 1.030 | 1.15x |
| signed-arithmetic-100 | 178.615 | 175.032 | 1.02x |

Outer signature recovery: 115.37 us median.

JIT + instantiation per pass: 651.54 ms, 966.29 ms, 489.52 ms.

Arithmetic-1000 pass medians: native 357.38, 369.62, 291.25 us; compiled 239.07, 238.26, 219.46 us.

## One-time compilation

Generation: 0.48 ms; compile/link: 7.332 s; Wasm size: 175764 bytes. One build observation, separate from warmed timing. The exact source hashes are retained in the raw report; bundle baseline_commit names its packaging parent, not a clean-source assertion.

## Methodology and limits

- Identical generic Linux executable and compiled.wasm bytes on both hosts, verified by SHA-256. Wasmtime JIT can select each host CPU features.
- Nine alternating-order batches per workload/backend/pass after warmup. The table uses the median of all27 timings; raw samples and per-pass medians are retained. Calibration targets ~25ms native batches, bounded to10–10000 calls (out-of-Energy cap200).
- Each call resets the in-memory Host/journal/storage and constructs a normalized outcome. No database, RPC, chain sync or fsync. Code analysis, compilation, JIT and instance setup are outside warmed calls.
- Requested worker limits on both hosts: CPU250% (2.5 cores), memory.high6GiB/max8GiB, tasks128. Timed execution is single-threaded. WSL own-cgroup counters are visible; Remote host counters are not, so destination inherited limits/throttling are not independently established.
- Source mainnet node/indexer stayed running; destination node/indexer stayed stopped. No pinning, turbo control or host/hypervisor isolation. Guest pressure and CPU flags are in raw data.
- Remote host wall-clock timing has substantial variability. Small differences around parity, especially storage/signed calls, do not establish a stable speedup. Ratios describe workloads/environments; they do not forecast full-block catch-up.
- Signature work is SHA-256, secp256k1 recovery, expected-key comparison and Keccak address hashing with a public synthetic fixture, identical before either backend. It is not a TVM precompile or broadcast transaction.

## Validation

The default compiler passed1549 raw-bytecode comparisons,180 Solidity comparisons,3960 boundary comparisons across17 programs,56 Lookups checks and2 signature checks. The portable harness adds192 comparisons per process pass (576 per host,1152 across both): status/output/Energy/storage/logs, factors0/1234/34000 and exact Energy boundaries +/-1. These checks do not independently establish full mainnet consensus equivalence.

## Reproduce

```sh
python3 scripts/fetch-wasi-sdk.py /path/to/tools
export TVM_WASI_SDK=/path/to/tools/wasi-sdk-34.0-x86_64-linux
python3 scripts/run.py check
python3 scripts/run.py boundaries
python3 scripts/run.py lookups
python3 scripts/run.py bench
cargo build --release --locked --example compare-host
python3 scripts/pack-host.py artifacts/host-bundle \
  --binary target/release/examples/compare-host \
  --wasm artifacts/Bench.wasm
# Copy/extract the same benchmark-bundle.tar.gz on each Linux host:
python3 bundle/run-host.py bundle results --host HOST_NAME --passes 3
# Collect unchanged result directories, then:
python3 scripts/summarize-hosts.py artifacts/baseline.json wsl-results xeon-results
```

Use a clean generic Linux Rust build environment for portability; adjust paths when CARGO_TARGET_DIR or TVM_ARTIFACTS is set. Receiving hosts need Python3 and compatible glibc/libgcc, with no compiler or node database. The runner verifies every manifest hash.

[Raw samples and source hashes](baseline.json) · [validation counts](validation.json) · [compiler design](../docs/compiler.md). Earlier implementations and measurements remain in Git history.
