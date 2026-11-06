# Keccak inside WebAssembly — 2026-11-06

The same RustCrypto Keccak is slower when compiled into this Wasm module than when called through the Rust host. The current host import remains the default. This is a diagnostic of hash placement, using the existing compiler/runtime unchanged.

## Measured result

AMD Ryzen 9 9950X / fast WSL. Medians of 27 batch samples (three process passes, nine samples each); lower is better. Compilation, Wasmtime JIT and instance setup are excluded.

| 64-byte hash kernel | Nanoseconds / hash |
| --- | ---: |
| Direct native Rust digest | 224.13 |
| Actual Wasm → Rust import | 257.25 |
| Separate Keccak function inside Wasm | 554.59 |

The in-Wasm kernel takes 2.16x the time of the existing import. The direct digest control omits the bridge and TVM Word packing; it is not an identical entry wrapper.

| Warmed full call | Native TVM us | Wasm + Rust import us | Wasm + local Keccak us | Local / import time |
| --- | ---: | ---: | ---: | ---: |
| raw-keccak64-1 | 0.640 | 0.663 | 0.976 | 1.47x |
| raw-keccak64-100 | 24.178 | 29.010 | 57.875 | 1.99x |
| solidity-hash-0 | 0.680 | 0.803 | 0.765 | 0.95x |
| solidity-hash-1 | 1.076 | 1.400 | 1.706 | 1.22x |
| solidity-hash-10 | 4.404 | 5.754 | 9.080 | 1.58x |
| solidity-hash-100 | 37.390 | 49.424 | 82.306 | 1.67x |
| solidity-hash-1000 | 369.237 | 486.915 | 814.720 | 1.67x |
| payload-1 | 0.677 | 0.679 | 0.948 | 1.40x |
| payload-135 | 0.677 | 0.761 | 1.048 | 1.38x |
| payload-136 | 0.890 | 0.977 | 1.569 | 1.61x |
| payload-137 | 0.885 | 0.976 | 1.570 | 1.61x |
| payload-1024 | 2.206 | 2.706 | 5.110 | 1.89x |
| payload-65536 | 106.007 | 134.900 | 278.035 | 2.06x |

Solidity hashLoop repeatedly hashes abi.encode(x,i), exactly 64 bytes per iteration. Raw opcode loops isolate most Solidity ABI/loop work. Payload cases include calldata copying, memory expansion and returning 32 bytes. Small differences in the zero-hash control are noise-sensitive and do not establish a speedup.

## What was compiled

- The host's Alloy Keccak uses RustCrypto sha3=0.11.0 / keccak=0.2.2 with the software backend on this x86_64 build. The diagnostic uses those same source versions and locked transitive packages, with alloc/oid convenience features disabled; no alternative hash algorithm is substituted.
- [The no_std Rust static library](wasm-keccak/src/lib.rs) uses legacy Keccak-256, not NIST SHA3-256. It writes the digest as the same little-endian 256-bit TVM Word as the import. It allocates no heap memory, imports no hash callback and makes no Wasm reentry.
- Its public h_keccak entry has #[inline(never)]. C++ and Rust are linked as separate objects with no cross-language LTO. The linked Bench module exports a distinct h_keccak function (index 12); llvm-objdump shows seven explicit call 12 sites. This verifies separation in the emitted Wasm, not an assertion about every later JIT decision.
- The benchmark relinks the same generated C++ object with the Rust archive. It asserts that host.keccak is absent in the local module and present in the control. There is no compiler/backend-version switch or duplicate TVM runtime.
- Bench grows from 175764 to 179639 bytes (+3875 bytes). Source and module SHA-256 fingerprints, the direct-call proof and every timing sample are in [the raw report](wasm-keccak.json).

## Interpretation and limits

Eliminating the import does not improve these workloads: the crypto computation inside this Wasm build costs more than the native computation plus bridge. The earlier [counted empty-import control](hash-analysis.md) also found the hot import alone inexpensive. These measurements do not identify an exact instruction-level cause and do not imply that every Wasm implementation or runtime is inherently slower.

Rust 1.91.1 compiles the hash for different targets (native x86_64 versus wasm32); Wasmtime 38.0.4/Cranelift then lowers Wasm to machine code. Allocation features, LLVM lowering, wrappers, generated code and runtime optimization are not identical native instructions. No SIMD-specific Keccak implementation, alternate JIT or native assembly implementation was tested here.

Batches rotate all three paths; the kernel performs 100000 hashes per sample, then full-call calibration targets 20 ms native batches, bounded to 20–10000 calls. Worker limits were CPU 250%, memory.high 6 GiB / max 8 GiB, tasks 128. The host was not pinned or isolated; source mainnet services stayed active, and destination services were untouched. The result is a hash experiment, not a forecast of block-sync throughput.

## Correctness and reproduction

Each canonical pass verifies 170 vector/payload/Energy scenarios against both compiled placements: fixed empty/abc Keccak vectors, lengths 0/1/31/32/33/64/135/136/137/271/272/273/1024/65536, factors 0/1234/34000 and exact-used Energy +/-1. All 13 timed cases also compare normalized native/import/local outcomes; the hash kernel checks the returned digest. A separate CLI run passed and reproduced byte-identical modules. These checks do not establish complete TVM consensus equivalence.

Requirements for this optional diagnostic: Python 3.11+, Rust 1.91.1 with wasm32-unknown-unknown, and the usual C++/Wasm linker. The ordinary compiler still uses its Rust host import.

```sh
rustup target add --toolchain 1.91.1 wasm32-unknown-unknown
python3 scripts/run.py wasm-hash
```

The command builds only the small no_std crypto crate for wasm32 and runs the normal native benchmark executable. It checks that every locked crypto dependency also appears at the same version/checksum in the root lockfile. Reports and object/module files go into ignored artifacts/wasm-keccak. Repeat in three fresh processes to reproduce the canonical aggregation; timings have no pass/fail threshold.
