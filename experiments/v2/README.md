# v2: outlined checks and C++ exceptions

V1 is preserved in `src/compiler.rs` and `runtime/runtime.hpp`. V2 is separate:
`src/compiler_v2.rs` and `runtime/runtime_v2.hpp`. The default CLI still selects v1.

Each generated instruction starts with one call:

```cpp
reserve<0x52>(1, 2, -2);  // opcode, static Energy, required inputs, net stack delta
memory_op<0x52>();
goto Pnext;
```

Opcode is a template argument; the remaining three values are arguments to a
`noinline` helper. It resets the opcode's raw-cost accumulator, charges Energy,
and checks the stack. Compile-time opcode branches and templated operation
helpers specialize opcode selection. Reset/charge/need ordering is unchanged.
Dynamic charges stay grouped within the same opcode, preserving rounding of the
contract Energy factor.

All failed primitives throw `VmExit`. One outer `try/catch` around the generated
function returns the VM status. There is no per-PC post-operation status branch.
TVM REVERT remains an ordinary VM result. The exported catch counter proves that
error fixtures actually cross this boundary. Shared Rust result normalization
discards storage/logs for failed calls.

This uses real standard Wasm exception instructions, libc++abi and libunwind,
not a trap substituted for C++ catch. Wasmtime 38.0.4 enables exceptions and
`gc-drc` for **both** compared variants. V1 has no C++ exception runtime. The
small WASI facade supplies empty arguments/environment and denies file I/O,
clock access and unexpected process exit. There is no database or filesystem host.

## Reproduce on Linux x86_64

Keep the standard Rust/linker prerequisites from the root README. The pinned
wasi-sdk 34 includes matching LLVM and exception-enabled runtime libraries.

```sh
python3 scripts/fetch-wasi-sdk.py /path/to/tools
export TVM_WASI_SDK=/path/to/tools/wasi-sdk-34.0-x86_64-linux
python3 scripts/run.py check
python3 scripts/run.py check-v2
python3 scripts/run.py lookups-v2
python3 scripts/run.py compare-v2
```

Python 3.12+ is needed by the SDK extraction helper. An existing compatible
compiler/sysroot can be supplied through `TVM_CLANG_V2` instead.

The comparison harness uses the **same Clang executable** for v1 and v2,
three alternating compile/link rounds and nine rotating-order warmed samples
of native TVM/v1/v2. Reports retain every sample, Wasm sizes and separate JIT
costs. Short calls, long arithmetic, hashes, storage, reverts and thrown
out-of-Energy calls are measured. One-time costs stay out of warmed timings.

Generated [Lookups v2 C++](../../examples/generated-v2/Lookups.cpp) and
[runtime](../../examples/generated-v2/runtime.hpp) can be compared with
[the v1 example](../../examples/generated/Lookups.cpp). Contract and inputs are
unchanged. See [measured results](../../benchmarks/v2-comparison.md).

The pinned SDK archive SHA-256 is
`b761e3a0721dbae9c09a0059e5fdb2bf917d1b4a8a7b430fb3b5aafb0984b2c4`.
Official [C++ exception build guidance](https://github.com/WebAssembly/wasi-sdk/blob/main/CppExceptions.md)
explains the required flags and exception-enabled runtime libraries.

V2 remains the same bounded single-frame POC as v1. This experiment changes no
deployed TRON services and does not establish consensus completeness.
