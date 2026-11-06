# v3: inline reservation for static straight-line regions

V3 is an opt-in experiment in `src/compiler_v3.rs` and `runtime/runtime_v3.hpp`.
V1 and v2 stay preserved; v1 remains the default. V3 uses `-fno-exceptions` and the
same freestanding link as v1. There is no libc++abi/libunwind in the v3 module.

## Generated execution

The decoder finds contiguous regions of supported pure instructions. Every
`JUMPDEST` starts a new region. JUMP/JUMPI, STOP and INVALID terminate a region.
GAS is isolated so its remaining-Energy observation stays exact. EXP, storage,
memory, copies, hashing, logs, RETURN/REVERT and unsupported instructions are
also isolated. Dynamic operations keep their existing opcode-local charges.

A successful multi-instruction region performs one inline `reserve_block` call,
then executes its specialized operations without per-op charge/stack checks.
Single-instruction regions use the original v1 path. Operation helpers remain
inline. Arbitrary jumps can enter only at validated JUMPDEST region starts.

Generation computes each region's minimum input stack and maximum stack growth
from prefix deltas. Preflight checks both before any grouped operations execute.
It also checks and subtracts the complete static Energy budget.

## Exact Energy and failure behavior

Contract dynamic-factor penalties are rounded **per opcode**, not per region.
For a cost histogram `(cost, count)`, the reserved budget is:

```
sum(count * (cost + floor(cost * factor / 10000)))
```

This differs from scaling the summed raw budget, which can overcharge small
instructions. No GAS observation or dynamic charge occurs inside a grouped region.
The next isolated instruction resets its own raw-cost rounding accumulator.

If preflight fails, an out-of-line cold helper replays the original resource
checks in instruction order: reset opcode cost, charge, check stack, advance
abstract stack depth. It stops at the first failure. It executes no word
operations or external effects. This is valid because grouped instructions are
pure, all costs/stack effects are static, and a potentially failing control-flow
instruction can occur only at the end. It preserves cases where out-of-Energy
precedes stack underflow/overflow, including a failure later in the region.

The successful reservation path is always inline; the cold helper shares failure
handling without expanding an entire duplicate fast program. Errors return ordinary
VM status codes. Neither path throws C++ exceptions.

## Reproduce

```
python3 scripts/run.py check-v3
python3 scripts/run.py boundaries-v3
python3 scripts/run.py lookups-v3
# Comparing against v2 additionally needs its pinned exception-capable SDK:
export TVM_WASI_SDK=/path/to/wasi-sdk-34.0-x86_64-linux
python3 scripts/run.py compare-v3
```

The comparison uses the same Clang and Wasmtime settings for all variants,
three rotated compile/link rounds and nine rotating-order warmed samples of native
TVM/v1/v2/v3. One-time compilation and JIT costs are excluded from warmed calls.
See [measurements](../../benchmarks/v3-comparison.md).

[Generated Lookups](../../examples/generated-v3/Lookups.cpp),
[runtime](../../examples/generated-v3/runtime.hpp), and
[region metadata](../../examples/generated-v3/Lookups.blocks.json) show the actual
output. The Solidity source, inputs and shared in-memory Rust Host are unchanged.

Boundary checks cover valid/invalid dynamic jumps, a JUMPDEST byte inside PUSH
data, taken and untaken JUMPI, GAS observations, exact Energy limits, per-op penalty
rounding, charge-before-stack failure precedence, stack overflow and storage/log
rollback. Each failing call is repeated on the reused instance.

This remains a bounded single-frame execution POC. It changes no deployed node or
indexer and does not establish complete mainnet consensus equivalence.
