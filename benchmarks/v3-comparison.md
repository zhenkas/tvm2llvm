# V3 comparison — Ryzen 9 9950X / WSL2

V3 batches static resource checks for straight-line regions and uses no C++ exceptions. On this fixture it improves normal execution over v1, reduces module size and compiles much faster. V1 remains the default while v3 is an opt-in experiment.

| One-time cost / size | v1 | v2 | v3 |
| --- | ---: | ---: | ---: |
| Clang + link median | 46.170 s | 5.531 s | 7.248 s |
| Wasm bytes | 315,883 | 490,194 | 175,764 |
| JIT + instantiation (single sample) | 153.22 ms | 104.26 ms | 60.14 ms |

V3 compile/link time is 6.37x shorter than v1. Its Wasm is 44.4% smaller than v1 and 64.1% smaller than v2. Binary section inspection finds an exception tag section in v2 and none in v1/v3.

Both freestanding variants use -fno-exceptions; v2 uses actual C++ exceptions with libc++abi/libunwind. All variants use the same Clang23.1.0-wasi-sdk and Wasmtime38.0.4 / Cranelift settings. Final linking is freestanding Rust-bundled lld for v1/v3 versus the SDK driver/runtime for v2. Three rotated compile/link rounds are retained in the raw report. JIT values are single observations, not repeated JIT benchmarks.

Warmed median microseconds per call, nine samples with rotating backend order. Includes fresh in-memory Host/journal and normalized outcomes; excludes compilation/JIT. Lower is better.

| Workload | Native TVM us | v1 us | v2 us | v3 us | v3 speedup over v1 |
| --- | ---: | ---: | ---: | ---: | ---: |
| arithmetic-0 | 0.669 | 0.929 | 1.223 | 0.787 | 1.18x |
| arithmetic-1 | 0.747 | 1.018 | 1.417 | 0.860 | 1.18x |
| arithmetic-10 | 1.305 | 1.456 | 2.284 | 1.234 | 1.18x |
| arithmetic-100 | 6.898 | 5.605 | 11.046 | 4.719 | 1.19x |
| arithmetic-1000 | 61.769 | 47.118 | 98.401 | 39.852 | 1.18x |
| hash-1 | 1.053 | 1.587 | 1.958 | 1.409 | 1.13x |
| hash-10 | 4.437 | 6.757 | 8.533 | 6.290 | 1.07x |
| hash-100 | 39.158 | 56.748 | 72.798 | 54.023 | 1.05x |
| branch-0 | 0.791 | 1.175 | 1.403 | 1.007 | 1.17x |
| branch-1 | 0.683 | 0.907 | 1.171 | 0.757 | 1.20x |
| branch-7 | 0.677 | 0.914 | 1.176 | 0.759 | 1.21x |
| storage-seed | 1.134 | 1.438 | 1.630 | 1.297 | 1.11x |
| token-transfer | 3.013 | 4.367 | 5.109 | 4.133 | 1.06x |
| gas-probe | 5.331 | 4.722 | 10.128 | 4.087 | 1.16x |
| arithmetic-100-dynamic-energy | 11.257 | 6.836 | 60.826 | 5.156 | 1.33x |
| out-of-energy-exception | 0.297 | 0.260 | 0.799 | 0.269 | 0.97x |

Normal included workloads improve by about 5–21% over v1; dynamic-Energy arithmetic improves by 33%. The out-of-Energy microcase is about 3.6% slower (0.260 versus 0.269 us), showing a small cold-path cost. Hash/storage workloads remain slower than the native interpreter. This does not establish faster whole-block synchronization.

The decoded Bench program groups 630 instructions into 126 multi-instruction regions, eliminating 504 repeated static reservation sites. These are whole-bytecode counts, not per-call execution counts.

Validation: 1,549 raw + 180 Solidity comparisons; 3,960 boundary comparisons across 17 programs; 56 Lookups mapping/array/hash checks + 2 signature checks; 576 comparisons across the four benchmark backends at exact Energy boundaries. Results, Energy, storage and logs matched. Boundary cases exercise GAS observations, factor rounding, jump destinations inside PUSH data, valid/invalid/taken/untaken jumps, failure precedence, overflow and rollback. Repeated generated Lookups artifacts are byte-identical.

[Raw report](v3-comparison.json) · [algorithm and reproduction](../experiments/v3/README.md) · [generated Lookups](../examples/generated-v3/Lookups.cpp) · [region metadata](../examples/generated-v3/Lookups.blocks.json).
