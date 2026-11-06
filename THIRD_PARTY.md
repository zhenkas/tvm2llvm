# Source and licenses

The transpiler, freestanding C++ runtime, Wasmtime bridge, benchmark driver and Solidity contracts are original work under LGPL-3.0-or-later.

The native reference is extracted from the maintained TRON Rust node at commit `19175ccff23ac6b4a2d619dc792cf241c1033758`. Its upstream is [gingerbreadfork/tron-goblin-node](https://github.com/gingerbreadfork/tron-goblin-node). `src/energy.rs` is adapted from that node's LGPL-3.0 energy schedule.

The three vendored revm crates preserve their upstream MIT license notices and the TRON patches from the LGPL node: `revm-interpreter`, `revm-context-interface`, and `revm-context`. The last crate provides a journal/context, not a persistent database. `vendor/SOURCE.json` records provenance, file hashes, and standalone manifest adaptations. Other dependencies retain their package licenses.

The specialization approach was informed by the README of [EF/CF evm2cpp](https://github.com/uni-due-syssec/efcf-evm2cpp). No source from that project was copied or linked. This is an independent implementation.
