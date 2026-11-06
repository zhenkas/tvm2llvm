// SPDX-License-Identifier: LGPL-3.0-or-later
//! The reference host's RustCrypto Keccak compiled into the contract's Wasm module.
#![no_std]
use sha3::{Digest, Keccak256};

#[panic_handler]
fn panic(_: &core::panic::PanicInfo<'_>) -> ! {
    core::arch::wasm32::unreachable()
}

/// Writes the hash as a little-endian TVM stack Word, matching the Rust host ABI.
///
/// # Safety
/// Generated callers supply a valid non-null input pointer for len bytes and
/// an output pointer valid for 32 bytes. The digest is completed before writes,
/// so input/output overlap is allowed. No allocation or Wasm reentry occurs.
#[no_mangle]
#[inline(never)]
pub unsafe extern "C" fn h_keccak(input: *const u8, len: usize, output: *mut u8) {
    let bytes = unsafe { core::slice::from_raw_parts(input, len) };
    let digest = Keccak256::digest(bytes);
    for i in 0..32 {
        unsafe { output.add(i).write(digest[31 - i]) };
    }
}
