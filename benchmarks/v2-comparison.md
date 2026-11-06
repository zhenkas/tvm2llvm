# V2 comparison on Ryzen 9 9950X / WSL2

The outlined, exception-based v2 compiles much faster and runs slower on every included workload. Keep v1 as the default. These are single-host POC measurements, not a mainnet speed estimate.

Clang 23.1.0-wasi-sdk from pinned wasi-sdk34 compiles both variants; Wasmtime38.0.4 / Cranelift with exceptions and gc-drc enabled runs both. V1 uses its freestanding Rust-bundled lld link; v2 links libc++abi/libunwind with the SDK driver. Three alternating compile/link rounds, median below.

| One-time cost | v1 | v2 |
| --- | ---: | ---: |
| Clang + link, median | 46.569 s | 5.307 s |
| Wasm size | 315,883 bytes | 490,194 bytes |
| JIT + instantiation, single sample | 151.77 ms | 98.61 ms |

V2 compile/link time is 8.78x shorter. Its module is larger because of the exception runtime. The JIT sample is reported separately and is not a robust repeated JIT benchmark.

Nine samples per workload, median microseconds per complete call. Backend order rotates each round. Includes fresh in-memory Host/result normalization; excludes compilation and JIT. A reused Wasm instance resets prior active memory. Lower time is better.

| Workload | Native TVM us | v1 us | v2 us | v2 / v1 time |
| --- | ---: | ---: | ---: | ---: |
| arithmetic-0 | 0.709 | 0.971 | 1.246 | 1.28x |
| arithmetic-1 | 0.782 | 1.085 | 1.394 | 1.29x |
| arithmetic-10 | 1.409 | 1.523 | 2.342 | 1.54x |
| arithmetic-100 | 7.455 | 5.894 | 11.433 | 1.94x |
| arithmetic-1000 | 67.656 | 49.849 | 102.741 | 2.06x |
| hash-1 | 1.131 | 1.700 | 2.061 | 1.21x |
| hash-10 | 4.333 | 7.025 | 8.503 | 1.21x |
| hash-100 | 36.946 | 55.822 | 71.474 | 1.28x |
| branch-0 | 0.800 | 1.106 | 1.378 | 1.25x |
| branch-1 | 0.667 | 0.913 | 1.128 | 1.24x |
| branch-7 | 0.673 | 0.906 | 1.111 | 1.23x |
| storage-seed | 1.153 | 1.346 | 1.551 | 1.15x |
| token-transfer | 2.955 | 4.332 | 4.956 | 1.14x |
| gas-probe | 5.222 | 4.625 | 8.301 | 1.79x |
| arithmetic-100-dynamic-energy | 11.011 | 6.724 | 58.793 | 8.74x |
| out-of-energy-exception | 0.295 | 0.275 | 0.768 | 2.79x |

The change trades optimizer visibility across instruction boundaries for a smaller repeated reservation path. Calls, context loads/stores and Energy accounting are plausible contributors to the slowdown; this is an inference, not an isolated attribution. The dynamic-Energy result is especially poor. Since outlining, opcode templates and exception handling changed together, these data do not prove that exceptions alone caused the difference.

Correctness: 1,549 raw + 180 Solidity differential cases passed for v2; 56 mapping/array/hash checks and 2 signature checks passed. The comparison harness added 504 native/v1/v2 boundary/error comparisons. Its explicit stack overflow, underflow, bad memory, storage rollback and log rollback fixtures reached the real C++ catch boundary. Repeated calls after errors matched the reference. V1 regression checks are retained in CI.

[Raw measurements](v2-comparison.json) contain every sample and all compile rounds. [Experiment and reproduction instructions](../experiments/v2/README.md). [Generated v2 example](../examples/generated-v2/Lookups.cpp).
