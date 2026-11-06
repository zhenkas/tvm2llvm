# Compiler and resource reservation

The compiler is `src/compiler.rs`; its only C++ runtime is `runtime/runtime.hpp`. Generated modules use `-fno-exceptions` and freestanding linking, with no libc++abi/libunwind or WASI imports. VM errors return ordinary status codes.

## Straight-line regions

The decoder groups contiguous supported pure instructions. Every JUMPDEST starts a new region. JUMP/JUMPI, STOP and INVALID terminate a region. GAS is isolated so its remaining-Energy observation stays exact. EXP, storage, memory, copies, hashing, logs, RETURN/REVERT and unsupported instructions are isolated. Dynamic operations retain opcode-local charges.

A multi-instruction region performs one inline `reserve_block`, then executes specialized operations without per-op charge/stack checks. Single instructions use opcode-local checks. Operation helpers remain inline. Arbitrary jumps enter only at validated JUMPDEST region starts.

Generation computes minimum input stack and maximum stack growth from prefix deltas. Preflight checks both before any grouped operation executes, then checks and subtracts the complete static Energy budget.

## Exact Energy and failure behavior

Dynamic-factor penalties are rounded per opcode. Given a static-cost histogram `(cost, count)`, the budget is:

```
sum(count * (cost + floor(cost * factor / 10000)))
```

Scaling the sum of raw costs can overcharge small instructions. No GAS observation or dynamic charge occurs inside a grouped region; the next isolated instruction resets its raw-cost rounding accumulator.

If preflight fails, the cold out-of-line `block_fault` helper replays resource checks in instruction order: reset opcode cost, charge, check stack, advance abstract depth. It stops at the first failure and executes no word operation or Host effect. Grouped operations are pure, their resource costs are static, and potentially failing control flow occurs only at the end. This preserves failure precedence, including out-of-Energy before stack underflow/overflow.

The successful path stays inline; the cold helper avoids generating a duplicate fast program. Neither path throws C++ exceptions. Wasmtime is built without its exception-specific GC feature or WASI support.

## Validation and examples

```
python3 scripts/run.py check
python3 scripts/run.py boundaries
python3 scripts/run.py lookups
```

Boundary checks exercise valid/invalid dynamic jumps, JUMPDEST bytes inside PUSH data, taken/untaken JUMPI, GAS, exact Energy limits, per-op rounding, failure precedence, stack overflow and storage/log rollback. Calls repeat on the reused instance.

[Generated Lookups](../examples/generated/Lookups.cpp), [runtime](../examples/generated/runtime.hpp), [region metadata](../examples/generated/Lookups.blocks.json) and [validation](../examples/generated/validation.json) show the real output. [Baseline measurements](../benchmarks/README.md) compare the native interpreter with this compiler.

This remains a bounded single-frame POC; differential checks do not establish complete mainnet consensus equivalence.
