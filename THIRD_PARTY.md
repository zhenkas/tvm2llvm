# Source and licenses

The transpiler, freestanding C++ runtime, Wasmtime bridge, benchmark driver and Solidity contracts are original work under LGPL-3.0-or-later.

The native reference is extracted from the maintained TRON Rust node at commit `19175ccff23ac6b4a2d619dc792cf241c1033758`. Its upstream is [gingerbreadfork/tron-goblin-node](https://github.com/gingerbreadfork/tron-goblin-node). `src/energy.rs` is adapted from that node's LGPL-3.0 energy schedule.

The three vendored revm crates preserve their upstream MIT license notices and the TRON patches from the LGPL node: `revm-interpreter`, `revm-context-interface`, and `revm-context`. The last crate provides a journal/context, not a persistent database. `vendor/SOURCE.json` records provenance, file hashes, and standalone manifest adaptations. Other dependencies retain their package licenses.

The specialization approach was informed by the README of [EF/CF evm2cpp](https://github.com/uni-due-syssec/efcf-evm2cpp). No source from that project was copied or linked. This is an independent implementation.

## Experimental v2 exception toolchain

V2 uses pinned wasi-sdk 34 (LLVM/Clang 23.1.0-wasi-sdk) to compile standard Wasm exceptions and link libc++abi/libunwind. These LLVM runtime libraries use Apache-2.0 with LLVM exceptions; their source/licenses are distributed with the upstream LLVM/wasi-sdk projects. The SDK is downloaded outside this repository, verified by SHA-256, and is not vendored. Generated Wasm/object files are ignored. V1 compiler/runtime sources are unchanged. See experiments/v2/README.md for the exact SDK and flags.
