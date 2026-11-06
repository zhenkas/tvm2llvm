// SPDX-License-Identifier: LGPL-3.0-or-later
//! V3: one inline reservation per safe straight-line region, no exceptions.
pub use crate::compiler::Compilation;
use crate::energy;
use anyhow::{bail, Context, Result};
use serde::Serialize;
use std::{collections::BTreeMap, fmt::Write as _, path::Path, process::Command, time::Instant};
#[derive(Debug)]
struct Instruction {
    pc: usize,
    op: u8,
    data: Vec<u8>,
}
#[derive(Debug, Serialize)]
pub struct Region {
    pub start: usize,
    pub end: usize,
    pub instructions: usize,
    pub batched: bool,
    pub raw_energy: u64,
    pub min_stack: i32,
    pub peak_growth: i32,
}
fn decode(code: &[u8]) -> Vec<Instruction> {
    let mut ins = vec![];
    let mut pc = 0;
    while pc < code.len() {
        let op = code[pc];
        let n = if (0x60..=0x7f).contains(&op) {
            usize::from(op - 0x5f)
        } else {
            0
        };
        let mut data = vec![0; n];
        let end = (pc + 1 + n).min(code.len());
        data[..end - pc - 1].copy_from_slice(&code[pc + 1..end]);
        ins.push(Instruction { pc, op, data });
        pc += 1 + n;
    }
    ins
}
fn stack(op: u8) -> (usize, i32) {
    match op {
        0x01..=0x07 | 0x0a..=0x0b | 0x10..=0x14 | 0x16..=0x18 | 0x1a..=0x1d => (2, -1),
        0x08..=0x09 => (3, -2),
        0x15 | 0x19 | 0x35 | 0x51 | 0x54 => (1, 0),
        0x20 => (2, -1),
        0x37 | 0x39 | 0x3e => (3, -3),
        0x50 | 0x56 => (1, -1),
        0x52 | 0x53 | 0x55 | 0x57 | 0xf3 | 0xfd => (2, -2),
        0x80..=0x8f => (usize::from(op - 0x7f), 1),
        0x90..=0x9f => (usize::from(op - 0x8f) + 1, 0),
        0xa0..=0xa4 => (usize::from(op - 0xa0) + 2, -i32::from(op - 0xa0) - 2),
        0x30
        | 0x32..=0x34
        | 0x36
        | 0x38
        | 0x3a
        | 0x3d
        | 0x41..=0x48
        | 0x58..=0x5a
        | 0x5f..=0x7f => (0, 1),
        _ => (0, 0),
    }
}
fn safe(op: u8) -> bool {
    matches!(op,0x00|0x01..=0x09|0x0b|0x10..=0x1d|0x30|0x32..=0x36|0x38|0x3a|0x3d|0x41..=0x48|0x50|0x56..=0x59|0x5b|0x5f..=0x9f|0xfe)
}
fn terminal(op: u8) -> bool {
    matches!(op, 0x00 | 0x56 | 0x57 | 0xfe)
}
fn ranges(ins: &[Instruction]) -> Vec<(usize, usize)> {
    let mut out = vec![];
    let mut i = 0;
    while i < ins.len() {
        let mut j = i + 1;
        if safe(ins[i].op) && !terminal(ins[i].op) {
            while j < ins.len() && safe(ins[j].op) && ins[j].op != 0x5b {
                let stop = terminal(ins[j].op);
                j += 1;
                if stop {
                    break;
                }
            }
        }
        out.push((i, j));
        i = j;
    }
    out
}
pub fn analyze(code: &[u8]) -> Vec<Region> {
    let ins = decode(code);
    let gas = energy::tron_static_gas_table(true);
    ranges(&ins)
        .into_iter()
        .map(|(a, b)| {
            let mut delta = 0i32;
            let mut min_stack = 0;
            let mut peak = 0;
            for i in &ins[a..b] {
                let (n, d) = stack(i.op);
                min_stack = min_stack.max(n as i32 - delta);
                delta += d;
                peak = peak.max(delta);
            }
            Region {
                start: ins[a].pc,
                end: ins[b - 1].pc + 1 + ins[b - 1].data.len(),
                instructions: b - a,
                batched: b - a > 1 && safe(ins[a].op),
                raw_energy: ins[a..b]
                    .iter()
                    .map(|i| u64::from(gas[i.op as usize]))
                    .sum(),
                min_stack,
                peak_growth: peak,
            }
        })
        .collect()
}
fn literal(bytes: &[u8]) -> String {
    let mut b = [0u8; 32];
    b[32 - bytes.len()..].copy_from_slice(bytes);
    let v: Vec<_> = b
        .chunks_exact(8)
        .rev()
        .map(|x| format!("{}ULL", u64::from_be_bytes(x.try_into().unwrap())))
        .collect();
    format!("Word{{{{{}}}}}", v.join(","))
}
fn body(i: &Instruction, code_len: usize) -> String {
    let (pc, op, data) = (&i.pc, &i.op, &i.data);
    let body = match op {
        0x00 => "return SUCCESS;".into(),
        0x01..=0x07 | 0x0a..=0x0b | 0x10..=0x14 | 0x16..=0x18 | 0x1a..=0x1d => {
            format!("binary({op});")
        }
        0x08..=0x09 => format!("ternary({op});"),
        0x15 | 0x19 => format!("unary({op});"),
        0x20 => "hash_op();".into(),
        0x30 | 0x32 | 0x33 => {
            let n = if *op == 0x30 { 2 } else { 1 };
            let mut b = [n; 32];
            b[..12].fill(0);
            // Modern MULTI_SIGN fixture mode exposes 20-byte addresses.
            b[11] = 0;
            format!("c.push({});", literal(&b))
        }
        0x34 | 0x3a | 0x3d | 0x41 | 0x44 | 0x45 | 0x47 => "c.push(Word{});".into(),
        0x35 => "calldata_load();".into(),
        0x36 => "c.push(Word::small(c.input_len));".into(),
        0x38 => format!("c.push(Word::small({}));", code_len),
        0x37 | 0x39 | 0x3e => format!("copy_op({op},CODE,{});", code_len),
        0x42 => "c.push(Word::small(1700000000));".into(),
        0x43 => "c.push(Word::small(1));".into(),
        0x46 => "c.push(Word::small(728126428));".into(),
        0x48 => "c.push(Word::small(100));".into(),
        0x50 => "c.pop();".into(),
        0x51..=0x53 => format!("memory_op({op});"),
        0x54 => format!("storage_op({op});"),
        0x55 => format!(
            "if(!c.charge({}))return c.status;storage_op({op});",
            energy::tron_gas_params_for(false).sstore_static_gas()
        ),
        0x56 => "c.jump=c.pop();goto DISPATCH;".into(),
        0x57 => "c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;".into(),
        0x58 => format!("c.push(Word::small({pc}));"),
        0x59 => "c.push(Word::small(c.mem_size));".into(),
        0x5a => "c.push(Word::small(c.remaining));".into(),
        0x5b => String::new(),
        0x5f => "c.push(Word{});".into(),
        0x60..=0x7f => format!("c.push({});", literal(data)),
        0x80..=0x8f => format!("c.push(c.stack[c.sp-{}]);", op - 0x7f),
        0x90..=0x9f => format!(
            "{{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-{}];c.stack[c.sp-{}]=t;}}",
            op - 0x8f + 1,
            op - 0x8f + 1
        ),
        0xa0..=0xa4 => format!(
            "if(!c.charge({}))return c.status;log_op({});",
            energy::tron_gas_params_for(false).log_cost(op - 0xa0, 0),
            op - 0xa0
        ),
        0xf3 => "finish(SUCCESS);return c.status;".into(),
        0xfd => "finish(REVERTED);return c.status;".into(),
        0xfe => "return INVALID;".into(),
        _ => "return UNSUPPORTED;".into(),
    };

    body
}
pub fn generate(code: &[u8]) -> String {
    let ins = decode(code);
    let gas = energy::tron_static_gas_table(true);
    let plan = analyze(code);
    let mut s = String::from("#include \"runtime.hpp\"\nstatic const uint8_t CODE[]={");
    for x in code {
        write!(s, "{x},").unwrap();
    }
    if code.is_empty() {
        s.push('0');
    }
    writeln!(s,"}};\nextern \"C\" __attribute__((visibility(\"default\"))) uint32_t execute(uint32_t len,uint64_t energy,int64_t factor){{c.reset(len,energy,factor);if(len>65536)return UNSUPPORTED;goto P0;").unwrap();
    for ((a, b), region) in ranges(&ins).into_iter().zip(plan.iter()) {
        let first = &ins[a];
        writeln!(s, "P{}:{{", first.pc).unwrap();
        if region.batched {
            writeln!(
                s,
                "// {} straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.",
                b - a
            )
            .unwrap();
            s.push_str("static constexpr BlockStep steps[]={");
            let mut counts = BTreeMap::<u16, u64>::new();
            for i in &ins[a..b] {
                let (n, d) = stack(i.op);
                let cost = gas[i.op as usize];
                write!(s, "{{{cost},{n},{d}}},").unwrap();
                if cost != 0 {
                    *counts.entry(cost).or_default() += 1;
                }
            }
            s.push_str("};\nstatic constexpr CostCount costs[]={");
            for (cost, count) in &counts {
                write!(s, "{{{cost},{count}}},").unwrap();
            }
            if counts.is_empty() {
                s.push_str("{0,0}");
            }
            writeln!(
                s,
                "}};\nif(!reserve_block({}, {}, {}, costs))return block_fault(steps,{});",
                region.raw_energy,
                region.min_stack,
                region.peak_growth,
                b - a
            )
            .unwrap();
            for i in &ins[a..b] {
                writeln!(
                    s,
                    "// PC {} opcode 0x{:02x}\n{}",
                    i.pc,
                    i.op,
                    body(i, code.len())
                )
                .unwrap();
            }
            writeln!(s, "goto P{};}}", region.end).unwrap();
        } else {
            let i = &ins[a];
            let (n, d) = stack(i.op);
            writeln!(
                s,
                "c.op_raw=0;if(!c.charge({})||!c.need({n},{d}))return c.status;",
                gas[i.op as usize]
            )
            .unwrap();
            writeln!(
                s,
                "{}\nif(c.status)return c.status;goto P{};}}",
                body(i, code.len()),
                region.end
            )
            .unwrap();
        }
    }
    let end = ins.last().map(|i| i.pc + 1 + i.data.len()).unwrap_or(0);
    writeln!(s, "P{end}:return SUCCESS;").unwrap();
    s.push_str("DISPATCH:if(c.jump.high())return BAD_JUMP;switch(c.jump.v[0]){\n");
    for i in &ins {
        if i.op == 0x5b {
            writeln!(s, "case {}:goto P{};", i.pc, i.pc).unwrap();
        }
    }
    s.push_str("default:return BAD_JUMP;}}\n");
    s
}
fn run(cmd: &mut Command) -> Result<()> {
    let r = cmd.output()?;
    if !r.status.success() {
        bail!("compiler failed: {}", String::from_utf8_lossy(&r.stderr));
    }
    Ok(())
}
pub fn compile(code: &[u8], dir: &Path, name: &str) -> Result<Compilation> {
    std::fs::create_dir_all(dir)?;
    let t = Instant::now();
    let cpp = dir.join(format!("{name}.cpp"));
    let wasm = dir.join(format!("{name}.wasm"));
    let obj = dir.join(format!("{name}.o"));
    std::fs::write(
        dir.join("runtime.hpp"),
        include_str!("../runtime/runtime_v3.hpp"),
    )?;
    std::fs::write(&cpp, generate(code))?;
    std::fs::write(
        dir.join(format!("{name}.blocks.json")),
        serde_json::to_string_pretty(&analyze(code))? + "\n",
    )?;
    let generation_ms = t.elapsed().as_secs_f64() * 1000.;
    let t = Instant::now();
    run(
        Command::new(std::env::var("TVM_CLANG").unwrap_or("clang++".into()))
            .args([
                "--target=wasm32-unknown-unknown",
                "-std=c++17",
                "-O3",
                "-ffreestanding",
                "-fno-builtin",
                "-fno-exceptions",
                "-fno-rtti",
                "-fvisibility=hidden",
                "-c",
            ])
            .arg(&cpp)
            .arg("-o")
            .arg(&obj),
    )?;
    let ld = std::env::var("TVM_WASM_LD").unwrap_or("wasm-ld".into());
    let mut cmd = Command::new(&ld);
    if ld.contains("rust-lld") {
        cmd.args(["-flavor", "wasm"]);
    }
    cmd.arg(&obj)
        .args([
            "--no-entry",
            "--export=execute",
            "--export=input_ptr",
            "--export=remaining_energy",
            "--export=output_ptr",
            "--export=output_len",
            "--export-memory",
            "--initial-memory=4194304",
            "--max-memory=4194304",
            "-z",
            "stack-size=262144",
            "-o",
        ])
        .arg(&wasm);
    run(&mut cmd).context("linking Wasm")?;
    Ok(Compilation {
        cpp,
        wasm: wasm.clone(),
        generation_ms,
        clang_ms: t.elapsed().as_secs_f64() * 1000.,
        wasm_bytes: std::fs::metadata(wasm)?.len(),
    })
}
