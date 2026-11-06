# Measured results

Long arithmetic loops: 1.19–1.29× faster; dynamic-Energy arithmetic: 1.55×. Short calls, hashes and storage remain slower. Signed arithmetic: 1.09×.

Host: AMD Ryzen 9 9950X, WSL2; Rust 1.91.1, Clang 18.1.3, Wasmtime 38.0.4. Two-core process quota; existing TRON services remained running. Medians of nine alternating batches, microseconds per complete in-memory call.

| Case | Rust TVM µs | C++/Wasm µs | Native / Wasm |
|---|---:|---:|---:|
| arithmetic-0 | 0.645 | 0.918 | 0.70× |
| arithmetic-1 | 0.733 | 1.023 | 0.72× |
| arithmetic-10 | 1.314 | 1.490 | 0.88× |
| arithmetic-100 | 6.963 | 5.865 | 1.19× |
| arithmetic-1000 | 63.487 | 49.251 | 1.29× |
| hash-1 | 1.060 | 1.600 | 0.66× |
| hash-10 | 4.470 | 6.736 | 0.66× |
| hash-100 | 38.105 | 57.221 | 0.67× |
| branch-0 | 0.794 | 1.130 | 0.70× |
| branch-1 | 0.714 | 0.942 | 0.76× |
| branch-7 | 0.687 | 0.929 | 0.74× |
| storage-seed | 1.168 | 1.422 | 0.82× |
| token-transfer | 3.108 | 4.414 | 0.70× |
| arithmetic-100-dynamic-energy | 10.671 | 6.899 | 1.55× |

A ratio above 1 means Wasm is faster. These are workload-specific results, not mainnet block throughput.

Compile once: generation 1.112 ms; Clang/link 28475.6 ms; Wasm size 326,388 bytes; Wasmtime JIT/instantiation 141.7 ms.

Synthetic outer transaction signature recovery and address hashing: 22.063 µs. Signed arithmetic-100: native 30.828 µs, Wasm 28.203 µs, ratio 1.09×.

Full samples: ryzen-9950x-wsl.json; initial implementation: baseline.json and baseline-runtime.hpp. Correctness: 1,477 raw bytecode checks plus 180 Solidity checks. Tests include dynamic Energy per-opcode rounding.

The optimization forces small helpers to inline and uses bounded 64-bit division for the Energy factor, avoiding compiler-generated wide-multiply support calls. The larger module and longer compile time are part of that tradeoff. Hashing and storage still cross Wasm/Rust boundaries; profiling and stack-to-SSA lowering would be useful next experiments.

## Separate v2 experiment

See [v2 comparison](v2-comparison.md) and [raw data](v2-comparison.json): outlined template reservation plus C++ exceptions substantially reduces compilation time but slows the measured workloads. V1 remains the default.

## Separate v3 experiment

[The v3 comparison](v3-comparison.md) batches static checks and removes C++ exceptions. It improves the included normal workloads over v1, reduces Wasm size and compiles faster. [Raw samples](v3-comparison.json) include all three compiled variants and the native interpreter.
