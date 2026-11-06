// Extracted unchanged TRON schedule from node commit19175cc, tron-tvm/src/energy.rs. LGPL-3.0-or-later.
use std::sync::OnceLock;

use revm_context_interface::cfg::gas_params::{GasId, GasParams};
use revm_interpreter::primitives::hardfork::SpecId;

/// Builds TRON's TVM energy schedule as a revm [`GasParams`].
///
/// TRON froze its energy costs at Ethereum's **Frontier** gas schedule
/// (SLOAD 50, CALL 40, SSTORE 20000/5000, EXP 10/byte, SELFDESTRUCT base 0,
/// no EIP-2929 warm/cold) while *separately* adopting modern opcodes
/// (CREATE2, PUSH0, transient storage, …). No single revm `SpecId` expresses
/// "Frontier gas + modern opcodes", so we build the dynamic gas table from
/// `FRONTIER` and pin the *gas* spec ([`GasParams::spec`]) to `FRONTIER`. The
/// interpreter then applies Frontier gas *logic* — simple SET/CLEAR/RESET
/// SSTORE metering, no EIP-2200 stipend sentry, no warm/cold cost — regardless
/// of the opcode spec carried in `Cfg::spec`.
///
/// On top of Frontier we apply TRON's handful of deltas:
/// * **No energy refunds.** TRON has neither the SSTORE-clear (15000) nor the
///   SELFDESTRUCT (24000) refund (confirmed against `EnergyCost.java`).
/// * **SELFDESTRUCT to a dead account costs `NEW_ACCT_CALL` (25000).** Frontier
///   charges nothing for this; TRON charges the new-account topup.
/// * **Energy is execution-only.** There is no 21000 per-transaction base and
///   no calldata token cost in energy — those are charged as bandwidth (net),
///   not energy — so the intrinsic is zeroed.
fn build_tron_gas_params() -> GasParams {
    let mut gp = GasParams::new_spec(SpecId::FRONTIER);
    gp.override_gas(
        [
            // TRON has no energy refunds.
            (GasId::sstore_clearing_slot_refund(), 0),
            (GasId::selfdestruct_refund(), 0),
            // SELFDESTRUCT to a non-existent (dead) account: NEW_ACCT_CALL.
            (GasId::new_account_cost_for_selfdestruct(), 25_000),
            // Energy excludes the per-tx 21000 base and the calldata token cost
            // (both are charged as bandwidth, not energy).
            (GasId::tx_base_stipend(), 0),
            (GasId::tx_token_cost(), 0),
        ]
        .into_iter(),
    );
    gp
}

/// TRON's TVM energy schedule (see [`build_tron_gas_params`]), cached. Cloning
/// is cheap — the 256-entry cost table is shared behind an `Arc`.
pub fn tron_gas_params() -> GasParams {
    static CACHE: OnceLock<GasParams> = OnceLock::new();
    CACHE.get_or_init(build_tron_gas_params).clone()
}

/// TRON's energy schedule, selecting whether CALL/CREATE gas-forwarding retains
/// 1/64 (EIP-150).
///
/// java-tron's `Program.getCallEnergy` retains the 1/64 ONLY when
/// `allowTvmCompatibleEvm() && getContractVersion() == 1`. With
/// `ALLOW_TVM_COMPATIBLE_EVM` off — true for all of mainnet to date, where
/// every live contract is version 0 — it forwards ALL available energy to the
/// child frame. revm's interpreter otherwise applies the 1/64 retention
/// unconditionally (its opcode spec is ≥ Tangerine), which starves a sub-call
/// sitting near the energy limit and OOGs it where java completes (e.g. a
/// BatchTransfer wrapping a USDT transfer at a tight `fee_limit`). We encode
/// "no retention" as a 0 divisor in the `call_stipend_reduction` slot.
///
/// NOTE: when the flag IS active, java retains only for version-1 (post-fork)
/// contracts; this per-execution selector would retain for ALL contracts. That
/// case is unreachable on mainnet (flag off) — refine with per-frame contract
/// version if the proposal ever activates.
pub fn tron_gas_params_for(retain_call_gas_64th: bool) -> GasParams {
    if retain_call_gas_64th {
        return tron_gas_params();
    }
    static NO_RETAIN: OnceLock<GasParams> = OnceLock::new();
    NO_RETAIN
        .get_or_init(|| {
            let mut gp = build_tron_gas_params();
            gp.override_gas([(GasId::call_stipend_reduction(), 0)].into_iter());
            gp
        })
        .clone()
}

/// TRON's static (per-opcode) energy table.
///
/// revm's Frontier static table already matches TRON's per-opcode costs
/// (SLOAD 50, CALL 40, EXP base 10, JUMPI 10, …). TRON has two deviations:
///
/// 1. **MLOAD / MSTORE / MSTORE8 carry a base of 1 post-#65** (plus memory
///    expansion), not Ethereum's VERY_LOW tier (3). Verified against java-tron
///    via `triggerconstantcontract` — with base 3 our `energy_used` ran exactly
///    `2 × (#MLOAD + #MSTORE)` high on every call (decimals +8, balanceOf +12,
///    string returns +30); base 1 makes it match exactly. But java
///    `OperationRegistry.adjustMemOperations` adds that `SPECIAL_TIER` base only
///    when `ALLOW_HIGHER_LIMIT_FOR_MAX_CPU_TIME_OF_ONE_TX` (#65) is active —
///    pre-#65 the three ops have NO base (memory expansion only), hence the
///    `allow_higher_limit` gate.
///
/// 2. **CODECOPY / CALLDATACOPY / RETURNDATACOPY carry NO base** (just memory
///    expansion + the 3-per-word copy cost), not Ethereum's VERY_LOW tier (3).
///    java-tron registers these with `EnergyCost::get{Code,CallData,ReturnData}
///    CopyCost`, each of which returns *only* `calcMemEnergy(...)` (mem + copy)
///    with no tier added — `OperationRegistry`'s `(opcode, 3, 0, …)` numbers are
///    the stack in/out arity, not a base. Literal EVM/revm bills a VERY_LOW(3)
///    base on top, so every copy op ran exactly +3 high: a clean multiple-of-3
///    `energy_usage_total` over-charge across the mainnet window (1 copy → +3,
///    11 copies → +33, 300 → +900) that desyncs energy-tight txs. Note the
///    asymmetry: EXTCODECOPY keeps its base (java `EXT_CODE_COPY = 20`) and
///    MCOPY keeps VERY_LOW (java `getMCopyCost = VERY_LOW_TIER + calcMemEnergy`),
///    so only the three bare copies are zeroed here.
pub fn tron_static_gas_table(allow_higher_limit: bool) -> [u16; 256] {
    // Opcodes: MLOAD=0x51, MSTORE=0x52, MSTORE8=0x53.
    const MLOAD: usize = 0x51;
    const MSTORE: usize = 0x52;
    const MSTORE8: usize = 0x53;
    // Bare copy opcodes (no base tier in java-tron): CALLDATACOPY=0x37,
    // CODECOPY=0x39, RETURNDATACOPY=0x3e.
    const CALLDATACOPY: usize = 0x37;
    const CODECOPY: usize = 0x39;
    const RETURNDATACOPY: usize = 0x3e;
    let mut table = revm_interpreter::gas_table();
    // The MLOAD/MSTORE/MSTORE8 SPECIAL_TIER base (1) is added only post-#65
    // (`ALLOW_HIGHER_LIMIT_FOR_MAX_CPU_TIME_OF_ONE_TX`); pre-#65 they carry
    // memory-expansion cost only — see the doc comment above.
    let mem_base = u16::from(allow_higher_limit);
    table[MLOAD] = mem_base;
    table[MSTORE] = mem_base;
    table[MSTORE8] = mem_base;
    table[CALLDATACOPY] = 0;
    table[CODECOPY] = 0;
    table[RETURNDATACOPY] = 0;
    table
}
