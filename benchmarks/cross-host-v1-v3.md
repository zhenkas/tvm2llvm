# Native TVM / v1 / v3 across two hosts — 2026-11-06

The same generic x86_64 Linux executable and the same precompiled Wasm bytes ran on both hosts. On the Remote host Xeon, v3 improves long arithmetic over v1 more than on the WSL Ryzen. It is not a universal improvement over the native TVM interpreter.

The reference is the TRON-patched Rust interpreter extracted from the node, executing the same Solidity runtime bytecode. Solidity itself is not a separate runtime. V1 and v3 execute their generated Wasm through Wasmtime/Cranelift.

| Workload | Xeon v3 / v1 speedup | WSL v3 / v1 speedup | Xeon v3 / TVM speedup | WSL v3 / TVM speedup |
| --- | ---: | ---: | ---: | ---: |
| arithmetic-100 | 1.74x | 1.18x | 1.52x | 1.44x |
| arithmetic-1000 | 1.68x | 1.18x | 1.48x | 1.56x |
| arithmetic-100-dynamic-energy | 1.85x | 1.34x | 3.68x | 2.13x |
| hash-100 | 1.21x | 1.05x | 0.75x | 0.69x |
| token-transfer | 1.18x | 1.08x | 0.96x | 0.73x |
| signed-arithmetic-100 | 1.14x | 1.04x | 1.02x | 1.08x |

A speedup below 1 means slower. The signed-call difference on Xeon is too small relative to the observed variability to establish an improvement. Signature-only host recovery takes median 126.19 us on Xeon versus 23.24 us on WSL. It uses a public synthetic test key, SHA-256, secp256k1 recovery, expected-key comparison and Keccak address hashing; it is not a TVM precompile.

## Warmed execution

Median microseconds per call from 27 batch timings for each backend and workload (three process passes, nine rotating-order samples per pass). Lower is better. Includes a fresh in-memory Host/journal, reset storage and normalized outcomes. Compilation and JIT/instantiation are excluded.

### Intel(R) Xeon(R) Silver 4216 CPU @ 2.10GHz

| Workload | Native TVM us | v1 us | v3 us | v3 / TVM speedup | v3 / v1 speedup |
| --- | ---: | ---: | ---: | ---: | ---: |
| arithmetic-0 | 3.774 | 4.909 | 4.007 | 0.94x | 1.22x |
| arithmetic-1 | 5.158 | 5.919 | 4.738 | 1.09x | 1.25x |
| arithmetic-10 | 7.970 | 9.074 | 6.393 | 1.25x | 1.42x |
| arithmetic-100 | 42.328 | 48.245 | 27.790 | 1.52x | 1.74x |
| arithmetic-1000 | 384.017 | 438.331 | 260.252 | 1.48x | 1.68x |
| hash-1 | 6.109 | 7.874 | 6.810 | 0.90x | 1.16x |
| hash-10 | 28.132 | 47.682 | 40.096 | 0.70x | 1.19x |
| hash-100 | 233.612 | 376.819 | 312.270 | 0.75x | 1.21x |
| branch-0 | 5.453 | 6.232 | 5.457 | 1.00x | 1.14x |
| branch-1 | 4.649 | 5.589 | 4.330 | 1.07x | 1.29x |
| branch-7 | 4.423 | 4.966 | 3.644 | 1.21x | 1.36x |
| storage-seed | 5.956 | 7.072 | 7.211 | 0.83x | 0.98x |
| token-transfer | 21.157 | 26.006 | 22.062 | 0.96x | 1.18x |
| gas-probe | 33.550 | 36.779 | 22.315 | 1.50x | 1.65x |
| arithmetic-100-dynamic-energy | 113.531 | 57.024 | 30.851 | 3.68x | 1.85x |
| out-of-energy-exception | 1.143 | 0.930 | 1.010 | 1.13x | 0.92x |
| signed-arithmetic-100 | 146.649 | 163.775 | 143.242 | 1.02x | 1.14x |

### AMD Ryzen 9 9950X 16-Core Processor

| Workload | Native TVM us | v1 us | v3 us | v3 / TVM speedup | v3 / v1 speedup |
| --- | ---: | ---: | ---: | ---: | ---: |
| arithmetic-0 | 0.682 | 0.980 | 0.842 | 0.81x | 1.16x |
| arithmetic-1 | 0.750 | 1.098 | 0.904 | 0.83x | 1.21x |
| arithmetic-10 | 1.323 | 1.503 | 1.256 | 1.05x | 1.20x |
| arithmetic-100 | 6.993 | 5.735 | 4.843 | 1.44x | 1.18x |
| arithmetic-1000 | 63.514 | 48.002 | 40.807 | 1.56x | 1.18x |
| hash-1 | 1.067 | 1.653 | 1.450 | 0.74x | 1.14x |
| hash-10 | 4.430 | 6.817 | 6.428 | 0.69x | 1.06x |
| hash-100 | 38.852 | 58.782 | 56.072 | 0.69x | 1.05x |
| branch-0 | 0.793 | 1.192 | 1.040 | 0.76x | 1.15x |
| branch-1 | 0.668 | 0.950 | 0.780 | 0.86x | 1.22x |
| branch-7 | 0.696 | 0.954 | 0.774 | 0.90x | 1.23x |
| storage-seed | 1.165 | 1.458 | 1.328 | 0.88x | 1.10x |
| token-transfer | 3.066 | 4.513 | 4.197 | 0.73x | 1.08x |
| gas-probe | 5.276 | 4.821 | 4.098 | 1.29x | 1.18x |
| arithmetic-100-dynamic-energy | 10.819 | 6.809 | 5.088 | 2.13x | 1.34x |
| out-of-energy-exception | 0.286 | 0.269 | 0.273 | 1.05x | 0.98x |
| signed-arithmetic-100 | 30.469 | 29.238 | 28.151 | 1.08x | 1.04x |

## Variability and practical limits

The WSL results were relatively stable. Remote host wall-clock samples varied substantially despite low guest CPU/memory pressure. For arithmetic-1000, the three pass medians were:

| Host | Native TVM us | v1 us | v3 us |
| --- | ---: | ---: | ---: |
| xeon-4216, pass 1 | 384.0 | 454.5 | 261.6 |
| xeon-4216, pass 2 | 360.3 | 385.2 | 226.5 |
| xeon-4216, pass 3 | 501.7 | 545.2 | 319.0 |
| wsl-ryzen-9950x, pass 1 | 63.6 | 48.0 | 40.9 |
| wsl-ryzen-9950x, pass 2 | 65.1 | 48.2 | 41.3 |
| wsl-ryzen-9950x, pass 3 | 63.0 | 47.6 | 40.2 |

All three Xeon passes favor v3 over native TVM and v1 on long arithmetic; their native/v3 ratios are 1.47–1.59x, and v1/v3 ratios 1.70–1.74x. Absolute Xeon times are less stable: individual arithmetic-1000 batches span approximately 284–711 us native, 324–1372 us v1 and 159–476 us v3. Do not interpret the displayed ratios as precise hardware constants. Short-call, storage and signature differences near parity are inconclusive on this host. The out-of-Energy case uses very short batches and is particularly sensitive to noise; its label is a VM exceptional halt, not C++ exception handling.

For this in-memory arithmetic-1000 fixture, Xeon median latency is 6.05x the WSL native latency and 6.38x the WSL v3 latency. The measured environment difference includes processor generation/clock, virtualization and scheduling; this experiment does not isolate those causes. There are no database reads/writes or fsync in the timed execution. It therefore demonstrates a CPU/execution/environment gap even without database storage, but does not predict whole-node catch-up speed.

## Bundle and environment

- Linux ELF: generic x86_64 release target, no target-cpu=native or RUSTFLAGS; Rust1.91.1, opt-level3, thinLTO, codegen-units1. Same executable bytes on both machines. Wasmtime38.0.4/Cranelift JIT may select each host CPU features.
- V1/v3 Wasm: same Clang23.1.0-wasi-sdk optimizer and freestanding link used in the [v3 experiment](v3-comparison.md); v1 315883 bytes, v3 175764 bytes; neither uses C++ exceptions.
- The runner requested the same limits on both hosts: 250% CPU (2.5 cores), memory.high6GiB/max8GiB, tasks128. Timed VM execution is single-threaded. WSL cgroup counters confirm these limits, no CPU throttling and no memory.high/OOM events. Remote host own-cgroup counters were not visible; this report does not claim independently verified destination cgroup limits or absence of inherited throttling.
- Source WSL mainnet node/indexer remained running in the background. Destination mainnet node and indexer remained stopped throughout. No production configuration or node code was changed.
- No CPU pinning, turbo control, hypervisor host isolation or frequency normalization was applied. Guest-visible pressure and CPU flags are retained in the raw report.
- Code generator baseline: c26c0bddc403511977e6d5e61ada63522c5a1a3c. The benchmark-only harness is included in the commit containing this report.

| File | SHA-256 |
| --- | --- |
| compare-host | 97061aa8d217b589026ae71c47e12f13b6c2c0bf2155f717c6399086f138eb78 |
| Bench.json | 689b2b36a8884fc12c1cc1be5eb488ce8efab3dd084ec58c7a00fcabd66a9a62 |
| v1.wasm | 1904902194d9899ed325129281143625304502d94dcdbdc614caadeaa81c948e |
| v3.wasm | 69020a522fac33861c542475b417b7c55711e153d36d047b077cc29476f30221 |

JIT + instantiation is reported separately for each process pass (milliseconds):

| Host | v1 passes | v3 passes |
| --- | --- | --- |
| xeon-4216 | 1942.0, 1645.4, 1120.2 | 1033.8, 1271.2, 438.3 |
| wsl-ryzen-9950x | 144.4, 142.2, 137.2 | 63.7, 62.1, 58.2 |

## Validation and reproduction

Each host passed 1152 differential comparisons (384 per process pass; 2304 total). Checks compare status, output, Energy, storage and logs at factors0/1234/34000 and exact used-Energy boundaries +/-1. Signed workloads use the same verified host signature path before all three backends. The warmed fixture and coverage restrictions remain those of the POC; this is not proof of full mainnet consensus equivalence.

Build the modules with the pinned SDK as described in [the v3 experiment](../experiments/v3/README.md). Use a clean generic Linux Rust build environment; unset target-specific RUSTFLAGS before building if portability is required. Pack once, copy the resulting archive unchanged, and run it on each host:

```sh
export TVM_WASI_SDK=/path/to/wasi-sdk-34.0-x86_64-linux
python3 scripts/run.py compare-v3
cargo build --release --locked --example compare-host
python3 scripts/pack-host.py artifacts/host-bundle \
  --binary target/release/examples/compare-host \
  --v1 artifacts/v3-comparison/v1-2/Bench.wasm \
  --v3 artifacts/v3-comparison/v3-2/Bench.wasm
# On each machine: copy/extract artifacts/host-bundle/benchmark-bundle.tar.gz, then:
python3 bundle/run-host.py bundle results --host HOST_NAME --passes 3
# Collect each host result directory unchanged and summarize:
python3 scripts/summarize-hosts.py artifacts/cross-host.json wsl-results xeon-results
```

If CARGO_TARGET_DIR or TVM_ARTIFACTS is set, adjust the binary/module paths accordingly. A receiving Linux host needs Python3 and compatible glibc/libgcc; it needs no Rust/C++ compiler, node database or RPC. The runner verifies every manifest SHA-256 before execution. It calibrates each host independently to target ~25ms native batches (10–10000 iterations, out-of-Energy cap200); medians are per-call time, not whole-batch duration.

[Raw 27-sample report, pass medians, JIT and host observations](cross-host-v1-v3.json) · [portable harness](../examples/compare-host.rs) · [runner](../scripts/run-host.py) · [packager](../scripts/pack-host.py) · [aggregator](../scripts/summarize-hosts.py).
