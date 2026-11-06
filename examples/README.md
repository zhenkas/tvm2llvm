# Как выглядит сгенерированный C++

[Исходный Solidity](Lookups.sol) → [полный настоящий вывод генератора](generated/Lookups.cpp).
[Runtime C++](generated/runtime.hpp) содержит inline-функции операций; [Rust Host](../src/wasm.rs) реализует storage и хеширование.
[Подпись](inspect-lookups.rs) проверяется отдельным Rust-кодом перед вызовом, не сгенерированной C++ функцией.

Генератор видит bytecode. В его выводе нет `balances.find()` или `values[index]`: есть стек, метки `P<pc>`, вычисление storage-ключа и SLOAD.
Сопоставление с Solidity ниже проверено по source map solc.
Блоки ниже — дословные фрагменты generated/Lookups.cpp.

## Резервирование прямого участка

Компилятор заранее считает статическую Energy и требования стека до следующей границы. Сначала выполняется один inline reserve_block, затем операции участка. Если бюджета не хватает, cold helper воспроизводит проверки в исходном порядке и возвращает точную ошибку; C++ exceptions не используются.

```cpp
P0:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,0,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 0 opcode 0x60
c.push(Word{{128ULL,0ULL,0ULL,0ULL}});
// PC 2 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P4;}
```

[План участков](generated/Lookups.blocks.json) и [описание алгоритма](../docs/compiler.md) относятся к этому же генератору. Хеширование, SLOAD и другие операции с динамической стоимостью остаются отдельными границами, как в примерах ниже.

## Mapping

```solidity
mapping(address => uint256) private balances; // slot 0
function mapGet(address who) external view returns (uint256) {
    return balances[who];
}
```

Ключ записи = `keccak256(abi.encode(who, uint256(0)))`: 32 байта адреса плюс 32 байта номера slot.
Затем SLOAD читает значение по этому ключу.

```cpp
P163:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P164;}
P164:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P165;}
```

`c.op_raw = 0` начинает новую группу округления динамической Energy.
`charge(30)` списывает базовую цену KECCAK256, `charge(50)` — SLOAD в выбранной TVM-модели.
`need` проверяет стек. `hash_op` дополнительно учитывает длину и расширение памяти.
`storage_op(84)` означает opcode 0x54/SLOAD, а не номер database-таблицы.

## Динамический массив

```solidity
uint256[] private values; // slot 1
function listGet(uint256 index) external view returns (uint256) {
    return values[index];
}
```

SLOAD(slot 1) получает длину, затем проверяется index < length.
Данные начинаются по ключу `keccak256(bytes32(uint256(1)))`.
Каждый uint256 занимает один slot: ключ элемента = base + index (256-битная арифметика).
У packed-массивов с мелкими типами размещение другое.

В полном C++: P174 читает длину; между P175 и P197 идут проверка границ и подготовка ключа;
P197 вычисляет base, P198 прибавляет index, P199 читает элемент.

```cpp
P174:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P175;}
// ... промежуточные инструкции находятся в полном файле ...
P197:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P198;}
P199:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P200;}
```

В фикстуре длина 3, данные [11, 22, 33]. index=1 возвращает 22.
index=3 и uint256::MAX дают Solidity Panic(0x32), выход за границы.

## Хеширование bytes

```solidity
function hashBytes(bytes calldata data) external pure returns (bytes32) {
    return keccak256(data);
}
```

После ABI-декодирования и копирования bytes в VM-память:

```cpp
P232:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P233;}
```

Runtime (только форматирование развёрнуто для чтения):

```cpp
inline void hash_op() {
    Word off = c.pop(), len = c.pop();
    uint32_t o, n;
    if (!c.range(off, len, o, n) ||
        !c.memory(o, n, 6 * ((uint64_t(n) + 31) / 32))) return;
    Word out{};
    h_keccak(pointer(c.mem + o), n, pointer(&out));
    c.push(out);
}
```

`h_keccak` — импорт Wasm из Rust Host. Сам Keccak сейчас выполняется общей native Rust-реализацией.
Он не переписан в сгенерированный C++; оба движка используют одну реализацию.
SLOAD аналогично вызывает `h_sload`, обращающийся к in-memory journal/Host.

```rust
// src/wasm.rs, обработчик host.keccak
let data = bytes(&mut c, p, n);
put(&mut c, v, U256::from_be_bytes(keccak256(data).0));
```

Для bytes "hello":
`1c8aff950685c2ed4bc3174f3472287b56d9517b9c948127319a09a7a36deac8`.

## Подпись

Подпись внешней транзакции проверяется в Rust до исполнения контракта.
В примере нет Solidity ecrecover: TVM precompile/CALL-путь пока не реализован.
Полная функция `verify_sender` — в [inspect-lookups.rs](inspect-lookups.rs).

SHA-256 raw bytes → secp256k1 public-key recovery → Keccak публичного ключа → сравнение 20-байтовой части адреса с ожидаемым отправителем.
Сам recover ещё не подтверждает ожидаемого отправителя: сравнение обязательно.
Префикс TRON 0x41, Base58 и permissions/multisig здесь не моделируются.
Вход — синтетические bytes, не полная сериализованная TRON-транзакция; отправки в сеть нет.

Проверено: исходные bytes с подписью принимаются; изменённые bytes с прежней подписью не совпадают с ожидаемым отправителем.

## Воспроизведение

```sh
# Фикстура уже сохранена; пересборка требует Node/npm:
npm ci --ignore-scripts --no-audit --no-fund
npm run compile:lookups

# Runtime bytecode -> C++ -> Wasm -> проверка вызовов:
python3 scripts/run.py lookups
```

[validation.json](generated/validation.json): 56 сравнений native/Wasm
(7 сценариев × 2 Energy-фактора × обычный и 3 граничных бюджета) плюс 2 проверки подписи.
C++ и runtime сохранены в Git; .o/.wasm пересобираются локально и исключены из Git.
