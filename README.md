# tvm2llvm — TVM → C++ → WebAssembly POC

A Rust project comparing the TRON-patched interpreter extracted from our Rust node with an independently written contract-specializing C++ compiler, compiled to WebAssembly and executed by Wasmtime. Both backends use the same in-memory Rust journal/Host. No RocksDB, RPC, chain sync or mainnet data is required.

**This is an execution/performance POC, not a consensus-ready replacement for TVM.** Unsupported executed opcodes fail explicitly. Production TRON services are not modified.

The project has one compiler and one freestanding runtime. Straight-line regions reserve static Energy and stack space once; dynamic operations preserve opcode-local checks. Helpers inline into generated code, and VM failures return status codes without C++ exceptions. See [compiler design](docs/compiler.md), [actual generated C++](examples/generated/Lookups.cpp) and [region metadata](examples/generated/Lookups.blocks.json).

## Run

Requirements: Rust1.91.1, Python3, Clang with wasm32 support, lld (or Rust's bundled rust-lld). Node.js24/npm are needed only to rebuild Solidity fixtures.

```sh
npm ci --ignore-scripts --no-audit --no-fund
npm run compile:fixtures
npm run compile:lookups
python3 scripts/run.py check
python3 scripts/run.py boundaries
python3 scripts/run.py lookups
python3 scripts/run.py bench
```

Optional environment variables: `TVM_CLANG`, `TVM_WASM_LD`, `TVM_WASI_SDK`, `TVM_ARTIFACTS`, `CARGO_TARGET_DIR`. Setting TVM_WASI_SDK selects its clang++ unless TVM_CLANG is explicit. The optional pinned [SDK downloader](scripts/fetch-wasi-sdk.py) verifies its SHA-256. The emitted module remains freestanding, with no WASI or C++ exception runtime.

Generated objects, Wasm and reports go into ignored `artifacts/`; the Lookups walkthrough is saved in `examples/generated/`. The benchmark reports generation, compile/link and JIT costs separately from warmed calls.

## Baseline and portable measurements

[Baseline results and methodology](benchmarks/README.md), [raw cross-host samples](benchmarks/baseline.json), and [validation counts](benchmarks/validation.json) capture the initial consolidated baseline at commit90625c8. Current hash-path measurements are reported separately below. Comparisons are native TVM versus compiled C++/Wasm. The portable harness runs the same Linux executable and contract module on both machines, verifies file hashes and preserves every sample. Earlier implementations and comparisons remain in Git history.

## Hash-path analysis

[Controlled hash measurements](benchmarks/hash-analysis.md) separate direct Keccak, a counted empty Wasm import, the actual hash callback, raw TVM opcodes and the Solidity loop. The bridge now borrows linear memory without a temporary Vec/copy; the remaining contract gap is reported explicitly. A separate [Keccak-inside-Wasm experiment](benchmarks/wasm-keccak.md) compiles the same crypto source as a noinline Wasm function and compares hash placement. It does not change the default host import.

## What is compared

- **Reference:** the node's TRON-patched revm interpreter with the extracted Frontier-based TVM Energy schedule, modern 1-Energy MLOAD/MSTORE/MSTORE8 and explicit modern fixture flags. Runtime bytecode analysis is reused across calls.
- **Compiled:** bytecode is decoded once into specialized C++ with fixed PCs. PUSH constants are embedded, static regions batch resource checks, and dynamic jumps dispatch only to validated JUMPDEST boundaries. Inline helpers implement 256-bit operations, memory and Energy accounting. Clang uses -O3; Wasmtime/Cranelift JIT compiles the module before warmed execution.
- **Shared Host:** storage and logs use the same Rust Context/Journal; Keccak uses the same Rust implementation. Each call resets initial storage. Success commits the returned view; revert/exception returns the initial view and discards logs.

Energy includes static/dynamic charges, memory expansion and a per-contract factor. Batched reservations preserve rounding per opcode. `GAS`/`gasleft()` sees the exact decremented counter. Checks compare status, output, Energy, storage and logs, including exact Energy boundaries and exceptional termination.

## Fixtures and coverage

`contracts/Bench.sol` is original Solidity: arithmetic loops, Keccak loops, branches/revert, token-style mapping updates, Transfer logs and gasleft(). It is compiled by pinned solc0.8.26 for the common Istanbul opcode subset; settings, ABI, bytecode and source hash are in `fixtures/Bench.json`. This does not claim coverage of TRON-only Solidity extensions.

Raw-bytecode checks cover 256-bit arithmetic, calldata/memory, storage, jumps, logs, truncated PUSH, invalid instructions and stack underflow. Boundary checks cover factor rounding, failure precedence, GAS, jump targets and rollback. [The Lookups walkthrough](examples/README.md) exercises mappings, arrays and hashing. Its outer-transaction signature example uses a public synthetic test key, SHA-256, secp256k1 recovery and address hashing. It neither broadcasts a transaction nor implements a TVM precompile.

Limits: one top-level frame, 3MiB VM memory, 64KiB calldata, nonnegative dynamic factor up to 1,000,000 and Energy at most i64::MAX. No nested CALL/CREATE, precompiles, external account/code reads, persistent storage, TRC-10/staking/governance opcodes, chain-state fixtures, block/resource settlement, wall-clock timeout consensus or full fork-history emulation. This bounded fixture model is not a live mainnet configuration.

## Provenance

See [THIRD_PARTY.md](THIRD_PARTY.md) and [vendor/SOURCE.json](vendor/SOURCE.json). The C++ compiler/runtime is original; no EF/CF evm2cpp source was copied. Dependencies and toolchain versions are pinned. LGPL-3.0-or-later applies to original code; vendored crates retain MIT licenses.
