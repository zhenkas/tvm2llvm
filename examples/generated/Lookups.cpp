#include "runtime.hpp"
static const uint8_t CODE[]={96,128,96,64,82,52,128,21,97,0,16,87,96,0,128,253,91,80,96,4,54,16,97,0,65,87,96,0,53,96,224,28,128,99,115,19,242,171,20,97,0,70,87,128,99,143,144,135,245,20,97,0,107,87,128,99,186,101,54,118,20,97,0,126,87,91,96,0,128,253,91,97,0,89,97,0,84,54,96,4,97,0,240,86,91,97,0,167,86,91,96,64,81,144,129,82,96,32,1,96,64,81,128,145,3,144,243,91,97,0,89,97,0,121,54,96,4,97,1,9,86,91,97,0,206,86,91,97,0,89,97,0,140,54,96,4,97,1,125,86,91,96,1,96,1,96,160,27,3,22,96,0,144,129,82,96,32,129,144,82,96,64,144,32,84,144,86,91,96,0,96,1,130,129,84,129,16,97,0,188,87,97,0,188,97,1,173,86,91,144,96,0,82,96,32,96,0,32,1,84,144,80,145,144,80,86,91,96,0,130,130,96,64,81,97,0,224,146,145,144,97,1,195,86,91,96,64,81,128,145,3,144,32,144,80,146,145,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,2,87,96,0,128,253,91,80,53,145,144,80,86,91,96,0,128,96,32,131,133,3,18,21,97,1,28,87,96,0,128,253,91,130,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,51,87,96,0,128,253,91,131,1,96,31,129,1,133,19,97,1,68,87,96,0,128,253,91,128,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,91,87,96,0,128,253,91,133,96,32,130,132,1,1,17,21,97,1,109,87,96,0,128,253,91,96,32,145,144,145,1,149,144,148,80,146,80,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,143,87,96,0,128,253,91,129,53,96,1,96,1,96,160,27,3,129,22,129,20,97,1,166,87,96,0,128,253,91,147,146,80,80,80,86,91,99,78,72,123,113,96,224,27,96,0,82,96,50,96,4,82,96,36,96,0,253,91,129,131,130,55,96,0,145,1,144,129,82,145,144,80,86,254,161,100,115,111,108,99,67,0,8,26,0,10,};
extern "C" __attribute__((visibility("default"))) uint32_t execute(uint32_t len,uint64_t energy,int64_t factor){c.reset(len,energy,factor);if(len>65536)return UNSUPPORTED;goto P0;
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
P4:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P5;}
P5:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{2,0,1},{3,1,1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{2,1},{3,3},{10,1},};
if(!reserve_block(21, 0, 3, costs))return block_fault(steps,5);
// PC 5 opcode 0x34
c.push(Word{});
// PC 6 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 7 opcode 0x15
unary(21);
// PC 8 opcode 0x61
c.push(Word{{16ULL,0ULL,0ULL,0ULL}});
// PC 11 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P12;}
P12:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 12 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 14 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P15;}
P15:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P16;}
P16:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{2,1,-1},{3,0,1},{2,0,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{2,2},{3,3},{10,1},};
if(!reserve_block(24, 1, 1, costs))return block_fault(steps,7);
// PC 16 opcode 0x5b

// PC 17 opcode 0x50
c.pop();
// PC 18 opcode 0x60
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
// PC 20 opcode 0x36
c.push(Word::small(c.input_len));
// PC 21 opcode 0x10
binary(16);
// PC 22 opcode 0x61
c.push(Word{{65ULL,0ULL,0ULL,0ULL}});
// PC 25 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P26;}
P26:{
// 9 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,0},{3,0,1},{3,2,-1},{3,1,1},{3,0,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{3,8},{10,1},};
if(!reserve_block(34, 0, 3, costs))return block_fault(steps,9);
// PC 26 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 28 opcode 0x35
calldata_load();
// PC 29 opcode 0x60
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
// PC 31 opcode 0x1c
binary(28);
// PC 32 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 33 opcode 0x63
c.push(Word{{1930687147ULL,0ULL,0ULL,0ULL}});
// PC 38 opcode 0x14
binary(20);
// PC 39 opcode 0x61
c.push(Word{{70ULL,0ULL,0ULL,0ULL}});
// PC 42 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P43;}
P43:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,1,1},{3,0,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{3,4},{10,1},};
if(!reserve_block(22, 1, 2, costs))return block_fault(steps,5);
// PC 43 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 44 opcode 0x63
c.push(Word{{2408613877ULL,0ULL,0ULL,0ULL}});
// PC 49 opcode 0x14
binary(20);
// PC 50 opcode 0x61
c.push(Word{{107ULL,0ULL,0ULL,0ULL}});
// PC 53 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P54;}
P54:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,1,1},{3,0,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{3,4},{10,1},};
if(!reserve_block(22, 1, 2, costs))return block_fault(steps,5);
// PC 54 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 55 opcode 0x63
c.push(Word{{3127195254ULL,0ULL,0ULL,0ULL}});
// PC 60 opcode 0x14
binary(20);
// PC 61 opcode 0x61
c.push(Word{{126ULL,0ULL,0ULL,0ULL}});
// PC 64 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P65;}
P65:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{1,1},{3,2},};
if(!reserve_block(7, 0, 2, costs))return block_fault(steps,3);
// PC 65 opcode 0x5b

// PC 66 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 68 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P69;}
P69:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P70;}
P70:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{2,0,1},{3,0,1},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,1},{3,4},{8,1},};
if(!reserve_block(23, 0, 5, costs))return block_fault(steps,7);
// PC 70 opcode 0x5b

// PC 71 opcode 0x61
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
// PC 74 opcode 0x61
c.push(Word{{84ULL,0ULL,0ULL,0ULL}});
// PC 77 opcode 0x36
c.push(Word::small(c.input_len));
// PC 78 opcode 0x60
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
// PC 80 opcode 0x61
c.push(Word{{240ULL,0ULL,0ULL,0ULL}});
// PC 83 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P84;}
P84:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{3,1},{8,1},};
if(!reserve_block(12, 0, 1, costs))return block_fault(steps,3);
// PC 84 opcode 0x5b

// PC 85 opcode 0x61
c.push(Word{{167ULL,0ULL,0ULL,0ULL}});
// PC 88 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P89;}
P89:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},};
static constexpr CostCount costs[]={{1,1},{3,1},};
if(!reserve_block(4, 0, 1, costs))return block_fault(steps,2);
// PC 89 opcode 0x5b

// PC 90 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P92;}
P92:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P93;}
P93:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,2,0},{3,2,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 2, 1, costs))return block_fault(steps,2);
// PC 93 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 94 opcode 0x81
c.push(c.stack[c.sp-2]);
goto P95;}
P95:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P96;}
P96:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,2,-1},{3,0,1},};
static constexpr CostCount costs[]={{3,3},};
if(!reserve_block(9, 1, 1, costs))return block_fault(steps,3);
// PC 96 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 98 opcode 0x01
binary(1);
// PC 99 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P101;}
P101:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P102;}
P102:{
// 4 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,1,1},{3,3,0},{3,2,-1},{3,2,0},};
static constexpr CostCount costs[]={{3,4},};
if(!reserve_block(12, 2, 1, costs))return block_fault(steps,4);
// PC 102 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 103 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 104 opcode 0x03
binary(3);
// PC 105 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P106;}
P106:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(SUCCESS);return c.status;
if(c.status)return c.status;goto P107;}
P107:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{2,0,1},{3,0,1},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,1},{3,4},{8,1},};
if(!reserve_block(23, 0, 5, costs))return block_fault(steps,7);
// PC 107 opcode 0x5b

// PC 108 opcode 0x61
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
// PC 111 opcode 0x61
c.push(Word{{121ULL,0ULL,0ULL,0ULL}});
// PC 114 opcode 0x36
c.push(Word::small(c.input_len));
// PC 115 opcode 0x60
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
// PC 117 opcode 0x61
c.push(Word{{265ULL,0ULL,0ULL,0ULL}});
// PC 120 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P121;}
P121:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{3,1},{8,1},};
if(!reserve_block(12, 0, 1, costs))return block_fault(steps,3);
// PC 121 opcode 0x5b

// PC 122 opcode 0x61
c.push(Word{{206ULL,0ULL,0ULL,0ULL}});
// PC 125 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P126;}
P126:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{2,0,1},{3,0,1},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,1},{3,4},{8,1},};
if(!reserve_block(23, 0, 5, costs))return block_fault(steps,7);
// PC 126 opcode 0x5b

// PC 127 opcode 0x61
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
// PC 130 opcode 0x61
c.push(Word{{140ULL,0ULL,0ULL,0ULL}});
// PC 133 opcode 0x36
c.push(Word::small(c.input_len));
// PC 134 opcode 0x60
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
// PC 136 opcode 0x61
c.push(Word{{381ULL,0ULL,0ULL,0ULL}});
// PC 139 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P140;}
P140:{
// 10 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{3,0,1},{3,2,-1},{3,2,-1},{3,2,-1},{3,0,1},{3,2,0},{3,2,1},};
static constexpr CostCount costs[]={{1,1},{3,9},};
if(!reserve_block(28, 1, 3, costs))return block_fault(steps,10);
// PC 140 opcode 0x5b

// PC 141 opcode 0x60
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
// PC 143 opcode 0x60
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
// PC 145 opcode 0x60
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
// PC 147 opcode 0x1b
binary(27);
// PC 148 opcode 0x03
binary(3);
// PC 149 opcode 0x16
binary(22);
// PC 150 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 152 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 153 opcode 0x81
c.push(c.stack[c.sp-2]);
goto P154;}
P154:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P155;}
P155:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,2,1},{3,2,0},};
static constexpr CostCount costs[]={{3,3},};
if(!reserve_block(9, 1, 2, costs))return block_fault(steps,3);
// PC 155 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 157 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 158 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P159;}
P159:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P160;}
P160:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,2,0},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 1, 1, costs))return block_fault(steps,2);
// PC 160 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
// PC 162 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P163;}
P163:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P164;}
P164:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P165;}
P165:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,2,0},{8,1,-1},};
static constexpr CostCount costs[]={{3,1},{8,1},};
if(!reserve_block(11, 2, 0, costs))return block_fault(steps,2);
// PC 165 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 166 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P167;}
P167:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{3,3,1},{3,2,1},};
static constexpr CostCount costs[]={{1,1},{3,4},};
if(!reserve_block(13, 1, 4, costs))return block_fault(steps,5);
// PC 167 opcode 0x5b

// PC 168 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 170 opcode 0x60
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
// PC 172 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 173 opcode 0x81
c.push(c.stack[c.sp-2]);
goto P174;}
P174:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P175;}
P175:{
// 4 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,2,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{3,3},{10,1},};
if(!reserve_block(19, 2, 1, costs))return block_fault(steps,4);
// PC 175 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 176 opcode 0x10
binary(16);
// PC 177 opcode 0x61
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
// PC 180 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P181;}
P181:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{3,2},{8,1},};
if(!reserve_block(14, 0, 2, costs))return block_fault(steps,3);
// PC 181 opcode 0x61
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
// PC 184 opcode 0x61
c.push(Word{{429ULL,0ULL,0ULL,0ULL}});
// PC 187 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P188;}
P188:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,2,0},{3,0,1},};
static constexpr CostCount costs[]={{1,1},{3,2},};
if(!reserve_block(7, 2, 1, costs))return block_fault(steps,3);
// PC 188 opcode 0x5b

// PC 189 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 190 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P192;}
P192:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P193;}
P193:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,0,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 193 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 195 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P197;}
P197:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P198;}
P198:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P199;}
P199:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P200;}
P200:{
// 6 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,2,0},{2,1,-1},{3,3,0},{3,2,0},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{2,2},{3,3},{8,1},};
if(!reserve_block(21, 4, 0, costs))return block_fault(steps,6);
// PC 200 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 201 opcode 0x50
c.pop();
// PC 202 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 203 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 204 opcode 0x50
c.pop();
// PC 205 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P206;}
P206:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,3,1},{3,3,1},{3,0,1},};
static constexpr CostCount costs[]={{1,1},{3,4},};
if(!reserve_block(13, 2, 4, costs))return block_fault(steps,5);
// PC 206 opcode 0x5b

// PC 207 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 209 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 210 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 211 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P213;}
P213:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P214;}
P214:{
// 6 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,4,0},{3,3,0},{3,2,0},{3,0,1},{8,1,-1},};
static constexpr CostCount costs[]={{3,5},{8,1},};
if(!reserve_block(23, 3, 2, costs))return block_fault(steps,6);
// PC 214 opcode 0x61
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
// PC 217 opcode 0x92
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
// PC 218 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 219 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 220 opcode 0x61
c.push(Word{{451ULL,0ULL,0ULL,0ULL}});
// PC 223 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P224;}
P224:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},};
static constexpr CostCount costs[]={{1,1},{3,1},};
if(!reserve_block(4, 0, 1, costs))return block_fault(steps,2);
// PC 224 opcode 0x5b

// PC 225 opcode 0x60
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P227;}
P227:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P228;}
P228:{
// 4 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,1,1},{3,3,0},{3,2,-1},{3,2,0},};
static constexpr CostCount costs[]={{3,4},};
if(!reserve_block(12, 2, 1, costs))return block_fault(steps,4);
// PC 228 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 229 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 230 opcode 0x03
binary(3);
// PC 231 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P232;}
P232:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P233;}
P233:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,2,0},{2,1,-1},{3,4,0},{3,3,0},{2,1,-1},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{2,3},{3,3},{8,1},};
if(!reserve_block(23, 5, 0, costs))return block_fault(steps,7);
// PC 233 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 234 opcode 0x50
c.pop();
// PC 235 opcode 0x92
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
// PC 236 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 237 opcode 0x50
c.pop();
// PC 238 opcode 0x50
c.pop();
// PC 239 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P240;}
P240:{
// 10 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{3,3,1},{3,5,1},{3,2,-1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,8},{10,1},};
if(!reserve_block(35, 2, 4, costs))return block_fault(steps,10);
// PC 240 opcode 0x5b

// PC 241 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 243 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 245 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 246 opcode 0x84
c.push(c.stack[c.sp-5]);
// PC 247 opcode 0x03
binary(3);
// PC 248 opcode 0x12
binary(18);
// PC 249 opcode 0x15
unary(21);
// PC 250 opcode 0x61
c.push(Word{{258ULL,0ULL,0ULL,0ULL}});
// PC 253 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P254;}
P254:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 254 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 256 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P257;}
P257:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P258;}
P258:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{2,1,-1},{3,1,0},{3,3,0},{3,2,0},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,2},{3,3},{8,1},};
if(!reserve_block(22, 4, 0, costs))return block_fault(steps,7);
// PC 258 opcode 0x5b

// PC 259 opcode 0x50
c.pop();
// PC 260 opcode 0x35
calldata_load();
// PC 261 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 262 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 263 opcode 0x50
c.pop();
// PC 264 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P265;}
P265:{
// 11 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,1,1},{3,0,1},{3,4,1},{3,6,1},{3,2,-1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,9},{10,1},};
if(!reserve_block(38, 2, 5, costs))return block_fault(steps,11);
// PC 265 opcode 0x5b

// PC 266 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 268 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 269 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 271 opcode 0x83
c.push(c.stack[c.sp-4]);
// PC 272 opcode 0x85
c.push(c.stack[c.sp-6]);
// PC 273 opcode 0x03
binary(3);
// PC 274 opcode 0x12
binary(18);
// PC 275 opcode 0x15
unary(21);
// PC 276 opcode 0x61
c.push(Word{{284ULL,0ULL,0ULL,0ULL}});
// PC 279 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P280;}
P280:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 280 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 282 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P283;}
P283:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P284;}
P284:{
// 9 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,3,1},{3,1,0},{3,0,1},{3,2,1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,7},{10,1},};
if(!reserve_block(32, 3, 3, costs))return block_fault(steps,9);
// PC 284 opcode 0x5b

// PC 285 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 286 opcode 0x35
calldata_load();
// PC 287 opcode 0x67
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
// PC 296 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 297 opcode 0x11
binary(17);
// PC 298 opcode 0x15
unary(21);
// PC 299 opcode 0x61
c.push(Word{{307ULL,0ULL,0ULL,0ULL}});
// PC 302 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P303;}
P303:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 303 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 305 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P306;}
P306:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P307;}
P307:{
// 10 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,4,1},{3,2,-1},{3,0,1},{3,2,1},{3,2,-1},{3,6,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,8},{10,1},};
if(!reserve_block(35, 5, 2, costs))return block_fault(steps,10);
// PC 307 opcode 0x5b

// PC 308 opcode 0x83
c.push(c.stack[c.sp-4]);
// PC 309 opcode 0x01
binary(1);
// PC 310 opcode 0x60
c.push(Word{{31ULL,0ULL,0ULL,0ULL}});
// PC 312 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 313 opcode 0x01
binary(1);
// PC 314 opcode 0x85
c.push(c.stack[c.sp-6]);
// PC 315 opcode 0x13
binary(19);
// PC 316 opcode 0x61
c.push(Word{{324ULL,0ULL,0ULL,0ULL}});
// PC 319 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P320;}
P320:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 320 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 322 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P323;}
P323:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P324;}
P324:{
// 9 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,1,1},{3,1,0},{3,0,1},{3,2,1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,7},{10,1},};
if(!reserve_block(32, 1, 3, costs))return block_fault(steps,9);
// PC 324 opcode 0x5b

// PC 325 opcode 0x80
c.push(c.stack[c.sp-1]);
// PC 326 opcode 0x35
calldata_load();
// PC 327 opcode 0x67
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
// PC 336 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 337 opcode 0x11
binary(17);
// PC 338 opcode 0x15
unary(21);
// PC 339 opcode 0x61
c.push(Word{{347ULL,0ULL,0ULL,0ULL}});
// PC 342 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P343;}
P343:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 343 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 345 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P346;}
P346:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P347;}
P347:{
// 11 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,6,1},{3,0,1},{3,3,1},{3,5,1},{3,2,-1},{3,2,-1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,9},{10,1},};
if(!reserve_block(38, 6, 4, costs))return block_fault(steps,11);
// PC 347 opcode 0x5b

// PC 348 opcode 0x85
c.push(c.stack[c.sp-6]);
// PC 349 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 351 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 352 opcode 0x84
c.push(c.stack[c.sp-5]);
// PC 353 opcode 0x01
binary(1);
// PC 354 opcode 0x01
binary(1);
// PC 355 opcode 0x11
binary(17);
// PC 356 opcode 0x15
unary(21);
// PC 357 opcode 0x61
c.push(Word{{365ULL,0ULL,0ULL,0ULL}});
// PC 360 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P361;}
P361:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 361 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 363 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P364;}
P364:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P365;}
P365:{
// 15 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,3,0},{3,2,0},{3,3,0},{3,2,-1},{3,7,0},{3,2,0},{3,6,0},{2,1,-1},{3,4,0},{2,1,-1},{2,1,-1},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,4},{3,9},{8,1},};
if(!reserve_block(44, 7, 1, costs))return block_fault(steps,15);
// PC 365 opcode 0x5b

// PC 366 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 368 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 369 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 370 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 371 opcode 0x01
binary(1);
// PC 372 opcode 0x95
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-7];c.stack[c.sp-7]=t;}
// PC 373 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 374 opcode 0x94
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-6];c.stack[c.sp-6]=t;}
// PC 375 opcode 0x50
c.pop();
// PC 376 opcode 0x92
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
// PC 377 opcode 0x50
c.pop();
// PC 378 opcode 0x50
c.pop();
// PC 379 opcode 0x50
c.pop();
// PC 380 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P381;}
P381:{
// 10 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{3,3,1},{3,5,1},{3,2,-1},{3,2,-1},{3,1,0},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,8},{10,1},};
if(!reserve_block(35, 2, 4, costs))return block_fault(steps,10);
// PC 381 opcode 0x5b

// PC 382 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 384 opcode 0x60
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
// PC 386 opcode 0x82
c.push(c.stack[c.sp-3]);
// PC 387 opcode 0x84
c.push(c.stack[c.sp-5]);
// PC 388 opcode 0x03
binary(3);
// PC 389 opcode 0x12
binary(18);
// PC 390 opcode 0x15
unary(21);
// PC 391 opcode 0x61
c.push(Word{{399ULL,0ULL,0ULL,0ULL}});
// PC 394 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P395;}
P395:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 395 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 397 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P398;}
P398:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P399;}
P399:{
// 14 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,2,1},{3,1,0},{3,0,1},{3,0,1},{3,0,1},{3,2,-1},{3,2,-1},{3,2,1},{3,2,-1},{3,2,1},{3,2,-1},{3,0,1},{10,2,-2},};
static constexpr CostCount costs[]={{1,1},{3,12},{10,1},};
if(!reserve_block(47, 2, 4, costs))return block_fault(steps,14);
// PC 399 opcode 0x5b

// PC 400 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 401 opcode 0x35
calldata_load();
// PC 402 opcode 0x60
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
// PC 404 opcode 0x60
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
// PC 406 opcode 0x60
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
// PC 408 opcode 0x1b
binary(27);
// PC 409 opcode 0x03
binary(3);
// PC 410 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 411 opcode 0x16
binary(22);
// PC 412 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 413 opcode 0x14
binary(20);
// PC 414 opcode 0x61
c.push(Word{{422ULL,0ULL,0ULL,0ULL}});
// PC 417 opcode 0x57
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P418;}
P418:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,1,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 418 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 420 opcode 0x80
c.push(c.stack[c.sp-1]);
goto P421;}
P421:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P422;}
P422:{
// 7 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,5,0},{3,4,0},{2,1,-1},{2,1,-1},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{1,1},{2,3},{3,2},{8,1},};
if(!reserve_block(21, 5, 0, costs))return block_fault(steps,7);
// PC 422 opcode 0x5b

// PC 423 opcode 0x93
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-5];c.stack[c.sp-5]=t;}
// PC 424 opcode 0x92
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
// PC 425 opcode 0x50
c.pop();
// PC 426 opcode 0x50
c.pop();
// PC 427 opcode 0x50
c.pop();
// PC 428 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P429;}
P429:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,0,1},{3,0,1},{3,2,-1},{3,0,1},};
static constexpr CostCount costs[]={{1,1},{3,4},};
if(!reserve_block(13, 0, 2, costs))return block_fault(steps,5);
// PC 429 opcode 0x5b

// PC 430 opcode 0x63
c.push(Word{{1313373041ULL,0ULL,0ULL,0ULL}});
// PC 435 opcode 0x60
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
// PC 437 opcode 0x1b
binary(27);
// PC 438 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P440;}
P440:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P441;}
P441:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,0,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 441 opcode 0x60
c.push(Word{{50ULL,0ULL,0ULL,0ULL}});
// PC 443 opcode 0x60
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P445;}
P445:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P446;}
P446:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,0,1},};
static constexpr CostCount costs[]={{3,2},};
if(!reserve_block(6, 0, 2, costs))return block_fault(steps,2);
// PC 446 opcode 0x60
c.push(Word{{36ULL,0ULL,0ULL,0ULL}});
// PC 448 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P450;}
P450:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P451;}
P451:{
// 4 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{1,0,0},{3,2,1},{3,4,1},{3,3,1},};
static constexpr CostCount costs[]={{1,1},{3,3},};
if(!reserve_block(10, 3, 3, costs))return block_fault(steps,4);
// PC 451 opcode 0x5b

// PC 452 opcode 0x81
c.push(c.stack[c.sp-2]);
// PC 453 opcode 0x83
c.push(c.stack[c.sp-4]);
// PC 454 opcode 0x82
c.push(c.stack[c.sp-3]);
goto P455;}
P455:{
c.op_raw=0;if(!c.charge(0)||!c.need(3,-3))return c.status;
copy_op(55,CODE,480);
if(c.status)return c.status;goto P456;}
P456:{
// 5 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{3,3,0},{3,2,-1},{3,2,0},{3,2,1},};
static constexpr CostCount costs[]={{3,5},};
if(!reserve_block(15, 2, 1, costs))return block_fault(steps,5);
// PC 456 opcode 0x60
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
// PC 458 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 459 opcode 0x01
binary(1);
// PC 460 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 461 opcode 0x81
c.push(c.stack[c.sp-2]);
goto P462;}
P462:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P463;}
P463:{
// 4 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,3,0},{3,2,0},{2,1,-1},{8,1,-1},};
static constexpr CostCount costs[]={{2,1},{3,2},{8,1},};
if(!reserve_block(16, 3, 0, costs))return block_fault(steps,4);
// PC 463 opcode 0x91
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
// PC 464 opcode 0x90
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
// PC 465 opcode 0x50
c.pop();
// PC 466 opcode 0x56
c.jump=c.pop();goto DISPATCH;
goto P467;}
P467:{
c.op_raw=0;if(!c.charge(0)||!c.need(0,0))return c.status;
return INVALID;
if(c.status)return c.status;goto P468;}
P468:{
c.op_raw=0;if(!c.charge(375)||!c.need(3,-3))return c.status;
if(!c.charge(375))return c.status;log_op(1);
if(c.status)return c.status;goto P469;}
P469:{
// 2 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{3,0,1},{0,0,0},};
static constexpr CostCount costs[]={{3,1},};
if(!reserve_block(3, 0, 1, costs))return block_fault(steps,2);
// PC 469 opcode 0x64
c.push(Word{{495790613315ULL,0ULL,0ULL,0ULL}});
// PC 475 opcode 0x00
return SUCCESS;
goto P476;}
P476:{
// 3 straight-line instructions; no interior JUMPDEST, GAS or dynamic charge.
static constexpr BlockStep steps[]={{8,3,-2},{3,2,-1},{0,0,0},};
static constexpr CostCount costs[]={{3,1},{8,1},};
if(!reserve_block(11, 4, 0, costs))return block_fault(steps,3);
// PC 476 opcode 0x08
ternary(8);
// PC 477 opcode 0x1a
binary(26);
// PC 478 opcode 0x00
return SUCCESS;
goto P479;}
P479:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-1))return c.status;
binary(10);
if(c.status)return c.status;goto P480;}
P480:return SUCCESS;
DISPATCH:if(c.jump.high())return BAD_JUMP;switch(c.jump.v[0]){
case 16:goto P16;
case 65:goto P65;
case 70:goto P70;
case 84:goto P84;
case 89:goto P89;
case 107:goto P107;
case 121:goto P121;
case 126:goto P126;
case 140:goto P140;
case 167:goto P167;
case 188:goto P188;
case 206:goto P206;
case 224:goto P224;
case 240:goto P240;
case 258:goto P258;
case 265:goto P265;
case 284:goto P284;
case 307:goto P307;
case 324:goto P324;
case 347:goto P347;
case 365:goto P365;
case 381:goto P381;
case 399:goto P399;
case 422:goto P422;
case 429:goto P429;
case 451:goto P451;
default:return BAD_JUMP;}}
