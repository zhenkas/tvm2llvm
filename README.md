# tvm2llvm — TVM → C++ → WebAssembly POC

A Rust experiment comparing the TRON-patched interpreter extracted from our Rust node with an independently written contract-specializing C++ generator, compiled to WebAssembly and executed by Wasmtime. Both backends use the same in-memory Rust journal/Host. No RocksDB, RPC, chain sync or mainnet data is required.

**This is an execution/performance POC, not a consensus-ready replacement for TVM.** Unsupported executed opcodes fail explicitly. The existing TRON services are not modified.

## Measured result

On the Ryzen 9 9950X WSL host: **Long arithmetic loops: 1.19–1.29× faster; dynamic-Energy arithmetic: 1.55×. Short calls, hashes and storage remain slower. Signed arithmetic: 1.09×.** This is a mixed result, not a general TVM acceleration claim.

See [the complete table and methodology](benchmarks/README.md), [current samples](benchmarks/ryzen-9950x-wsl.json), and [initial baseline](benchmarks/baseline.json). Its original header is retained in benchmarks/baseline-runtime.hpp; substitute it for runtime/runtime.hpp in a clean checkout to rerun the initial variant. Current compile cost is about 28.5 seconds with Clang and 142 ms for Wasmtime JIT/instantiation. Those one-time costs are excluded from warmed call timings.

## Run

Requirements: Rust 1.91.1, Python 3, Clang with wasm32 support, lld (or Rust's bundled rust-lld). Node.js 24/npm are needed only to rebuild the checked-in Solidity fixture.

```sh
npm ci --ignore-scripts --no-audit --no-fund
npm run compile:fixtures
python3 scripts/run.py check
python3 scripts/run.py bench
```

Optional environment variables: `TVM_CLANG`, `TVM_WASM_LD`, `TVM_ARTIFACTS`, `CARGO_TARGET_DIR`. Generated C++, object files, Wasm and reports go into ignored `artifacts/` by default. Benchmark JSON is `artifacts/report.json`; differential-check evidence is `artifacts/validation.json`.

## Inspect actual generated code

See [the mapping/array/hash walkthrough and signature example](examples/README.md), [Solidity source](examples/Lookups.sol), and [complete generated C++](examples/generated/Lookups.cpp). Rebuild and validate with `python3 scripts/run.py lookups`; regenerate the Solidity fixture with `npm run compile:lookups`.

## Separate v2 experiment

An opt-in [v2 experiment](experiments/v2/README.md) moves instruction reservation into a `noinline reserve<Op>(cost, inputs, delta)` helper and handles failures with one C++ `try/catch`. V1 remains the default. The comparison uses identical Clang and Wasmtime settings for both variants and includes failure-path costs.

## Separate v3 experiment

[V3](experiments/v3/README.md) batches static Energy and stack checks for straight-line regions, restores inline helpers and uses ordinary error returns without C++ exceptions. GAS, jump destinations and dynamic-cost operations preserve their exact boundaries. [Measurements](benchmarks/v3-comparison.md) compare native TVM and all three compiled variants.

## What is compared

- **Reference:** the node's TRON-patched revm interpreter with the extracted Frontier-based TVM Energy schedule, modern 1-Energy MLOAD/MSTORE/MSTORE8, and explicit modern fixture flags. Runtime bytecode analysis is reused across calls.
- **Compiled:** bytecode is decoded once; each instruction becomes specialized C++ with a fixed PC. PUSH constants are embedded; static branches become labels, and dynamic jumps use a validated JUMPDEST dispatch. Inline helpers implement 256-bit operations, memory, and Energy accounting. C++ is compiled with Clang `-O3`; Wasmtime/Cranelift compiles the module once before warmed execution.
- **Shared Host:** storage and logs use the same Rust Context/Journal; Keccak uses the same Rust implementation. Calls reset their initial storage. Success commits the returned view; revert/exception returns the initial view and discards logs.

Energy includes static and dynamic charges, memory expansion and a per-contract factor. Split charges within an opcode share a single rounding group. `GAS`/`gasleft()` sees the decremented counter. Tests compare status, output, Energy, storage and logs, including exact Energy boundaries, revert and exceptional termination.

## Fixtures and coverage

`contracts/Bench.sol` is original Solidity: arithmetic loops, Keccak loops, branches/revert, token-style mapping updates and Transfer logs, and `gasleft()`. It is compiled by pinned solc 0.8.26 for the common Istanbul opcode subset; compiler settings, ABI, bytecode and source hash are in `fixtures/Bench.json`. This does not claim coverage of TRON-only Solidity extensions.

Raw-bytecode checks exercise 256-bit arithmetic (signed/unsigned, overflow, shifts, modular arithmetic, exponentiation), calldata/memory, storage, jumps, logs, truncated PUSH, invalid instructions and stack underflow. The outer-transaction signature microbenchmark uses a **synthetic public test key**, SHA-256 digest, secp256k1 public-key recovery and address hashing. Signed-call timing runs the identical recovery path before either backend; it does not broadcast a transaction or implement a TVM ecrecover precompile.

Limits: one top-level frame, 3 MiB VM memory, 64 KiB calldata, nonnegative dynamic factor up to 1,000,000 and Energy at most i64::MAX. No nested CALL/CREATE, precompiles, external account/code reads, persistent storage, TRC-10/staking/governance opcodes, chain-state fixtures, block/resource settlement, wall-clock timeout consensus, or full fork-history emulation. The bounded environment is a fixture model, not a live mainnet configuration.

## Measurement

Each sample includes a fresh in-memory Host/journal, frame execution and normalized result construction. Code analysis, C++ compilation, Wasmtime JIT and module instantiation are excluded from warmed timings and reported separately. One Wasm instance is reused, resetting its prior active memory before each call. Nine sample batches alternate backend order after warmup; the report includes every sample and median microseconds per call. Inputs and outputs cross `black_box` barriers. Signature work is reported separately and in a signed arithmetic call.

These measurements establish only the included workloads on the recorded host. They do not predict block-sync speed, database performance or speed on every contract. Correctness against the extracted interpreter is a useful differential check, not independent proof of TVM consensus equivalence.

## Provenance

See [THIRD_PARTY.md](THIRD_PARTY.md) and [vendor/SOURCE.json](vendor/SOURCE.json). The C++ generator/runtime is original; no EF/CF evm2cpp source was copied. Dependencies and toolchain versions are pinned for reproduction. LGPL-3.0-or-later applies to the original project code; vendored crates retain MIT licenses.
