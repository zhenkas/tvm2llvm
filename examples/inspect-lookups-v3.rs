// SPDX-License-Identifier: LGPL-3.0-or-later
//! Generate an inspectable contract and compare map/array/hash calls with the native TVM.
use anyhow::{ensure, Result};
use revm_interpreter::primitives::{keccak256, U256};
use secp256k1::{
    ecdsa::{RecoverableSignature, RecoveryId},
    Message, Secp256k1, SecretKey,
};
use serde_json::{json, Value};
use sha2::{Digest, Sha256};
use std::{collections::BTreeMap, path::Path};
use tvm2llvm::{
    compiler_v3 as compiler,
    model::{Call, Outcome},
    native::NativeRunner,
    wasm::WasmRunner,
};

/// Outer transaction signature path. This is Rust host code, not generated C++.
/// The expected sender is supplied by the account/transaction, NOT derived from the signature.
fn verify_sender(
    raw_transaction: &[u8],
    signature: &[u8; 65],
    expected_sender: [u8; 20],
) -> Result<bool> {
    let digest: [u8; 32] = Sha256::digest(raw_transaction).into();
    let message = Message::from_digest(digest);
    let recovery_id = RecoveryId::from_i32(i32::from(signature[64]))?;
    let signature = RecoverableSignature::from_compact(&signature[..64], recovery_id)?;
    let public_key = Secp256k1::new()
        .recover_ecdsa(&message, &signature)?
        .serialize_uncompressed();
    let address = keccak256(&public_key[1..]);
    Ok(address[12..] == expected_sender)
}
fn selector(signature: &str) -> Vec<u8> {
    keccak256(signature.as_bytes())[..4].to_vec()
}
fn args(signature: &str, words: &[U256]) -> Vec<u8> {
    let mut input = selector(signature);
    for word in words {
        input.extend(word.to_be_bytes::<32>());
    }
    input
}
fn bytes_arg(signature: &str, data: &[u8]) -> Vec<u8> {
    let mut input = args(signature, &[U256::from(32), U256::from(data.len())]);
    input.extend(data);
    input.resize(4 + 64 + data.len().div_ceil(32) * 32, 0);
    input
}
fn signature_demo() -> Result<Value> {
    // Public synthetic fixture key. It has no funds and is never used for an account.
    let secp = Secp256k1::new();
    let key = SecretKey::from_slice(&[7; 32])?;
    let public = secp256k1::PublicKey::from_secret_key(&secp, &key).serialize_uncompressed();
    let expected: [u8; 20] = keccak256(&public[1..])[12..].try_into()?;
    let raw = b"synthetic transaction: transfer 17";
    let message = Message::from_digest(Sha256::digest(raw).into());
    let (id, compact) = secp
        .sign_ecdsa_recoverable(&message, &key)
        .serialize_compact();
    let mut signature = [0; 65];
    signature[..64].copy_from_slice(&compact);
    signature[64] = id.to_i32() as u8;
    let valid = verify_sender(raw, &signature, expected)?;
    let tampered = verify_sender(b"synthetic transaction: transfer 18", &signature, expected)?;
    ensure!(
        valid && !tampered,
        "signature fixture must accept original and reject changed message"
    );
    Ok(
        json!({"valid_message":valid,"tampered_message":tampered,"path":"Rust host, outer transaction; not a TVM precompile"}),
    )
}
fn assert_result(name: &str, result: &Outcome, expected: Option<U256>) -> Result<()> {
    match expected {
        Some(value) => {
            ensure!(result.status == "success", "{name}: {}", result.status);
            ensure!(
                result.output == hex::encode(value.to_be_bytes::<32>()),
                "{name}: incorrect value"
            );
        }
        None => ensure!(
            result.status == "revert",
            "{name}: bounds check did not revert"
        ),
    }
    Ok(())
}
fn main() -> Result<()> {
    let fixture: Value = serde_json::from_slice(&std::fs::read("fixtures/Lookups.json")?)?;
    let code = hex::decode(fixture["runtime"].as_str().unwrap())?;
    let dir = Path::new("examples/generated-v3");
    std::fs::create_dir_all(dir)?;
    let compiled = compiler::compile(&code, dir, "Lookups")?;
    let native = NativeRunner::new(&code);
    let mut wasm = WasmRunner::new(&compiled.wasm)?;
    // mapping balances is at slot 0. Its address key is right-aligned in 32 bytes.
    let who = [3u8; 20];
    let mut map_preimage = [0; 64];
    map_preimage[12..32].copy_from_slice(&who);
    let mapping_key = U256::from_be_bytes(keccak256(map_preimage).0);
    // A dynamic array's slot stores its length; elements start at keccak256(slot).
    let array_base = U256::from_be_bytes(keccak256(U256::from(1).to_be_bytes::<32>()).0);
    let mut storage = BTreeMap::new();
    storage.insert(mapping_key, U256::from(42));
    storage.insert(U256::from(1), U256::from(3));
    for (i, value) in [11u64, 22, 33].into_iter().enumerate() {
        storage.insert(array_base + U256::from(i), U256::from(value));
    }
    let cases = [
        (
            "mapping-hit",
            args("mapGet(address)", &[U256::from_be_slice(&who)]),
            Some(U256::from(42)),
        ),
        (
            "mapping-miss",
            args("mapGet(address)", &[U256::from(4)]),
            Some(U256::ZERO),
        ),
        (
            "array-index-1",
            args("listGet(uint256)", &[U256::from(1)]),
            Some(U256::from(22)),
        ),
        (
            "array-out-of-bounds",
            args("listGet(uint256)", &[U256::from(3)]),
            None,
        ),
        (
            "array-huge-index",
            args("listGet(uint256)", &[U256::MAX]),
            None,
        ),
        (
            "hash-hello",
            bytes_arg("hashBytes(bytes)", b"hello"),
            Some(U256::from_be_bytes(keccak256(b"hello").0)),
        ),
        (
            "hash-empty",
            bytes_arg("hashBytes(bytes)", b""),
            Some(U256::from_be_bytes(keccak256(b"").0)),
        ),
    ];
    let mut checks = 0;
    let mut examples = vec![];
    for (name, input, expected) in cases {
        for factor in [0, 34000] {
            let call = Call {
                input: input.clone(),
                initial_storage: storage.clone(),
                dynamic_factor: factor,
                ..Default::default()
            };
            let a = native.run(&call)?;
            let b = wasm.run(&call)?;
            ensure!(a == b, "{name}: native/Wasm differ\n{a:?}\n{b:?}");
            assert_result(name, &a, expected)?;
            checks += 1;
            if factor == 0 {
                examples.push(
                    json!({"case":name,"status":a.status,"output":a.output,"energy":a.energy_used}),
                );
            }
            for limit in [
                a.energy_used.saturating_sub(1),
                a.energy_used,
                a.energy_used + 1,
            ] {
                let call = Call {
                    energy_limit: limit,
                    ..call.clone()
                };
                ensure!(
                    native.run(&call)? == wasm.run(&call)?,
                    "{name}: Energy boundary {limit}"
                );
                checks += 1;
            }
        }
    }
    let signature = signature_demo()?;
    let report = json!({"passed":true,"native_wasm_checks":checks,"signature_checks":2,"examples":examples,"signature":signature,
        "storage":{"mapping_slot":0,"mapping_key":format!("{mapping_key:064x}"),"array_slot":1,"array_length":3,"array_base":format!("{array_base:064x}")}});
    std::fs::write(
        dir.join("validation.json"),
        serde_json::to_string_pretty(&report)? + "\n",
    )?;
    println!("{}", serde_json::to_string_pretty(&report)?);
    Ok(())
}
