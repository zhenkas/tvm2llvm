# Hash-path analysis — 2026-11-06

Both execution paths call the same Rust Keccak implementation. The earlier hashLoop table measured the whole TVM instruction/contract path, not just the hash algorithm. A minimal Wasm import control is cheap in this environment; the extra cost is in the callback and generated instruction/contract work.

## Measured controls

AMD Ryzen9 9950X / WSL2; Rust1.91.1, Clang23.1.0-wasi-sdk34, Wasmtime38.0.4/Cranelift. Three process passes with nine rotated-order batches each; all27 samples are retained. Lower is better.

| Control,64-byte input | Before ns | After ns |
| --- | ---: | ---: |
| Direct Rust Keccak | 223.28 | 223.78 |
| Empty imported callback + checked counter | 1.55 | 1.54 |
| Keccak through the actual WasmRunner callback | 290.44 | 254.48 |

The empty control has the same three-i32 argument arity. Its Rust counter verifies exactly1200000 imported calls per process pass, so the loop is not optimized away. This is a minimal hot-cache callback control, not a promise that every possible import costs the same. These are calls within one process, not IPC or system calls.

The hash microkernel calls the actual WasmRunner import100000 times on64 zero bytes, verifies the returned digest and omits TVM resource accounting. The direct control invokes the same primitive on the same bytes. All Wasm modules are byte-identical before/after; the changed host callback is bound by embedded source SHA-256.

## Whole-frame comparisons

Median microseconds per complete call, including Host/journal setup, TVM state/resource work and normalized results. Raw KECCAK256 programs remove Solidity ABI/loop encoding. The Solidity loop hashes abi.encode(bytes32,uint256),64 bytes per iteration.

| Workload | Native before us | Compiled before us | Native after us | Compiled after us |
| --- | ---: | ---: | ---: | ---: |
| raw-keccak64-1 | 0.662 | 0.730 | 0.630 | 0.655 |
| raw-keccak64-100 | 24.271 | 33.242 | 24.154 | 28.970 |
| solidity-hash-0 | 0.691 | 0.791 | 0.670 | 0.796 |
| solidity-hash-1 | 1.089 | 1.431 | 1.101 | 1.405 |
| solidity-hash-10 | 4.452 | 6.287 | 4.513 | 5.793 |
| solidity-hash-100 | 37.867 | 54.310 | 38.499 | 49.939 |
| solidity-hash-1000 | 373.958 | 529.309 | 379.294 | 486.002 |

## Concrete callback change

Previously src/wasm.rs allocated/zeroed a Vec, copied the input from linear memory, resolved the memory export while reading, hashed the temporary buffer, then resolved memory again to write the result. Native TVM passes a borrowed memory slice directly to the same Keccak primitive.

The callback now resolves memory once, hashes a checked borrowed slice, ends that borrow and writes the32-byte result using the same handle. It allocates no temporary input buffer and uses no unsafe code. Borrowing is confined to the synchronous hash operation: it does not re-enter Wasm or grow memory. Error handling traps invalid ranges explicitly. Digest endianness, TVM Energy, state and contract output are unchanged.

This controlled change removes allocation/copy and one export lookup together; it does not assign an independent nanosecond cost to each component.

## Remaining gap

The callback improvement reduces both raw-opcode and Solidity timings, but does not erase the difference from native TVM. The raw-opcode rows retain generated stack, range, memory and Energy work; the Solidity rows additionally retain ABI encoding, memory allocation in the VM and loop/control flow. The experiment separates those layers but is not an exact additive cycle profile. It does not support blaming the Keccak algorithm or an inherently expensive Wasm-to-Rust transition. Identifying the remaining instruction-level cost needs a further controlled profile; it is not claimed fixed here.

## Correctness and reproduction

Every pass checks132 native/compiled payload+Energy cases: sizes0,1,31,32,33,64,135,136,137,1024,65536, three dynamic factors and exact Energy boundaries. Expected digests are checked independently of TVM output formatting. The normal bytecode, Solidity, boundary, Lookups and signature suites are also rerun. No timing ratio is asserted in CI.

```sh
export TVM_WASI_SDK=/path/to/wasi-sdk-34.0-x86_64-linux
python3 scripts/run.py hash
```

Raw output defaults to ignored artifacts/hash-costs/report.json; TVM_ARTIFACTS selects another directory. [Raw before/after samples](hash-analysis.json), [Rust diagnostic](../examples/hash-costs.rs), [hash microkernel](host-hash.cpp), [empty control](host-noop.cpp), [current bridge](../src/wasm.rs). The [starting baseline](README.md) remains the initial consolidated snapshot, with its original source fingerprints.
