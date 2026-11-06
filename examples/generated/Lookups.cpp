#include "runtime.hpp"
static const uint8_t CODE[]={96,128,96,64,82,52,128,21,97,0,16,87,96,0,128,253,91,80,96,4,54,16,97,0,65,87,96,0,53,96,224,28,128,99,115,19,242,171,20,97,0,70,87,128,99,143,144,135,245,20,97,0,107,87,128,99,186,101,54,118,20,97,0,126,87,91,96,0,128,253,91,97,0,89,97,0,84,54,96,4,97,0,240,86,91,97,0,167,86,91,96,64,81,144,129,82,96,32,1,96,64,81,128,145,3,144,243,91,97,0,89,97,0,121,54,96,4,97,1,9,86,91,97,0,206,86,91,97,0,89,97,0,140,54,96,4,97,1,125,86,91,96,1,96,1,96,160,27,3,22,96,0,144,129,82,96,32,129,144,82,96,64,144,32,84,144,86,91,96,0,96,1,130,129,84,129,16,97,0,188,87,97,0,188,97,1,173,86,91,144,96,0,82,96,32,96,0,32,1,84,144,80,145,144,80,86,91,96,0,130,130,96,64,81,97,0,224,146,145,144,97,1,195,86,91,96,64,81,128,145,3,144,32,144,80,146,145,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,2,87,96,0,128,253,91,80,53,145,144,80,86,91,96,0,128,96,32,131,133,3,18,21,97,1,28,87,96,0,128,253,91,130,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,51,87,96,0,128,253,91,131,1,96,31,129,1,133,19,97,1,68,87,96,0,128,253,91,128,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,91,87,96,0,128,253,91,133,96,32,130,132,1,1,17,21,97,1,109,87,96,0,128,253,91,96,32,145,144,145,1,149,144,148,80,146,80,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,143,87,96,0,128,253,91,129,53,96,1,96,1,96,160,27,3,129,22,129,20,97,1,166,87,96,0,128,253,91,147,146,80,80,80,86,91,99,78,72,123,113,96,224,27,96,0,82,96,50,96,4,82,96,36,96,0,253,91,129,131,130,55,96,0,145,1,144,129,82,145,144,80,86,254,161,100,115,111,108,99,67,0,8,26,0,10,};
extern "C" __attribute__((visibility("default"))) uint32_t execute(uint32_t len,uint64_t energy,int64_t factor){c.reset(len,energy,factor);if(len>65536)return UNSUPPORTED;goto P0;
P0:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{128ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P2;}
P2:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P4;}
P4:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P5;}
P5:{
c.op_raw=0;if(!c.charge(2)||!c.need(0,1))return c.status;
c.push(Word{});
if(c.status)return c.status;goto P6;}
P6:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P7;}
P7:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P8;}
P8:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{16ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P11;}
P11:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P12;}
P12:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P14;}
P14:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P15;}
P15:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P16;}
P16:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P17;}
P17:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P18;}
P18:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P20;}
P20:{
c.op_raw=0;if(!c.charge(2)||!c.need(0,1))return c.status;
c.push(Word::small(c.input_len));
if(c.status)return c.status;goto P21;}
P21:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(16);
if(c.status)return c.status;goto P22;}
P22:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{65ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P25;}
P25:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P26;}
P26:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P28;}
P28:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
calldata_load();
if(c.status)return c.status;goto P29;}
P29:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P31;}
P31:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(28);
if(c.status)return c.status;goto P32;}
P32:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P33;}
P33:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1930687147ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P38;}
P38:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(20);
if(c.status)return c.status;goto P39;}
P39:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{70ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P42;}
P42:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P43;}
P43:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P44;}
P44:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{2408613877ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P49;}
P49:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(20);
if(c.status)return c.status;goto P50;}
P50:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{107ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P53;}
P53:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P54;}
P54:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P55;}
P55:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{3127195254ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P60;}
P60:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(20);
if(c.status)return c.status;goto P61;}
P61:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{126ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P64;}
P64:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P65;}
P65:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P66;}
P66:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P68;}
P68:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P69;}
P69:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P70;}
P70:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P71;}
P71:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P74;}
P74:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{84ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P77;}
P77:{
c.op_raw=0;if(!c.charge(2)||!c.need(0,1))return c.status;
c.push(Word::small(c.input_len));
if(c.status)return c.status;goto P78;}
P78:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P80;}
P80:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{240ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P83;}
P83:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P84;}
P84:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P85;}
P85:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{167ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P88;}
P88:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P89;}
P89:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P90;}
P90:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P92;}
P92:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P93;}
P93:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P94;}
P94:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P95;}
P95:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P96;}
P96:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P98;}
P98:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P99;}
P99:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P101;}
P101:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P102;}
P102:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P103;}
P103:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P104;}
P104:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P105;}
P105:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P106;}
P106:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(SUCCESS);return c.status;
if(c.status)return c.status;goto P107;}
P107:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P108;}
P108:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P111;}
P111:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{121ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P114;}
P114:{
c.op_raw=0;if(!c.charge(2)||!c.need(0,1))return c.status;
c.push(Word::small(c.input_len));
if(c.status)return c.status;goto P115;}
P115:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P117;}
P117:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{265ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P120;}
P120:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P121;}
P121:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P122;}
P122:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{206ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P125;}
P125:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P126;}
P126:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P127;}
P127:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P130;}
P130:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{140ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P133;}
P133:{
c.op_raw=0;if(!c.charge(2)||!c.need(0,1))return c.status;
c.push(Word::small(c.input_len));
if(c.status)return c.status;goto P134;}
P134:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P136;}
P136:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{381ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P139;}
P139:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P140;}
P140:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P141;}
P141:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P143;}
P143:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P145;}
P145:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P147;}
P147:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(27);
if(c.status)return c.status;goto P148;}
P148:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P149;}
P149:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(22);
if(c.status)return c.status;goto P150;}
P150:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P152;}
P152:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P153;}
P153:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P154;}
P154:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P155;}
P155:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P157;}
P157:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P158;}
P158:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P159;}
P159:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P160;}
P160:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P162;}
P162:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P163;}
P163:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P164;}
P164:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P165;}
P165:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P166;}
P166:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P167;}
P167:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P168;}
P168:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P170;}
P170:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P172;}
P172:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P173;}
P173:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P174;}
P174:{
c.op_raw=0;if(!c.charge(50)||!c.need(1,0))return c.status;
storage_op(84);
if(c.status)return c.status;goto P175;}
P175:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P176;}
P176:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(16);
if(c.status)return c.status;goto P177;}
P177:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P180;}
P180:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P181;}
P181:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P184;}
P184:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{429ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P187;}
P187:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P188;}
P188:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P189;}
P189:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P190;}
P190:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P192;}
P192:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P193;}
P193:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P195;}
P195:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P197;}
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
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P201;}
P201:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P202;}
P202:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P203;}
P203:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P204;}
P204:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P205;}
P205:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P206;}
P206:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P207;}
P207:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P209;}
P209:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P210;}
P210:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P211;}
P211:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P213;}
P213:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P214;}
P214:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P217;}
P217:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
if(c.status)return c.status;goto P218;}
P218:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P219;}
P219:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P220;}
P220:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{451ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P223;}
P223:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P224;}
P224:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P225;}
P225:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P227;}
P227:{
c.op_raw=0;if(!c.charge(1)||!c.need(1,0))return c.status;
memory_op(81);
if(c.status)return c.status;goto P228;}
P228:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P229;}
P229:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P230;}
P230:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P231;}
P231:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P232;}
P232:{
c.op_raw=0;if(!c.charge(30)||!c.need(2,-1))return c.status;
hash_op();
if(c.status)return c.status;goto P233;}
P233:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P234;}
P234:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P235;}
P235:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
if(c.status)return c.status;goto P236;}
P236:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P237;}
P237:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P238;}
P238:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P239;}
P239:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P240;}
P240:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P241;}
P241:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P243;}
P243:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P245;}
P245:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P246;}
P246:{
c.op_raw=0;if(!c.charge(3)||!c.need(5,1))return c.status;
c.push(c.stack[c.sp-5]);
if(c.status)return c.status;goto P247;}
P247:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P248;}
P248:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(18);
if(c.status)return c.status;goto P249;}
P249:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P250;}
P250:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{258ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P253;}
P253:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P254;}
P254:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P256;}
P256:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P257;}
P257:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P258;}
P258:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P259;}
P259:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P260;}
P260:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
calldata_load();
if(c.status)return c.status;goto P261;}
P261:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P262;}
P262:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P263;}
P263:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P264;}
P264:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P265;}
P265:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P266;}
P266:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P268;}
P268:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P269;}
P269:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P271;}
P271:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,1))return c.status;
c.push(c.stack[c.sp-4]);
if(c.status)return c.status;goto P272;}
P272:{
c.op_raw=0;if(!c.charge(3)||!c.need(6,1))return c.status;
c.push(c.stack[c.sp-6]);
if(c.status)return c.status;goto P273;}
P273:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P274;}
P274:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(18);
if(c.status)return c.status;goto P275;}
P275:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P276;}
P276:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{284ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P279;}
P279:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P280;}
P280:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P282;}
P282:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P283;}
P283:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P284;}
P284:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P285;}
P285:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P286;}
P286:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
calldata_load();
if(c.status)return c.status;goto P287;}
P287:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P296;}
P296:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P297;}
P297:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(17);
if(c.status)return c.status;goto P298;}
P298:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P299;}
P299:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{307ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P302;}
P302:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P303;}
P303:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P305;}
P305:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P306;}
P306:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P307;}
P307:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P308;}
P308:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,1))return c.status;
c.push(c.stack[c.sp-4]);
if(c.status)return c.status;goto P309;}
P309:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P310;}
P310:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{31ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P312;}
P312:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P313;}
P313:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P314;}
P314:{
c.op_raw=0;if(!c.charge(3)||!c.need(6,1))return c.status;
c.push(c.stack[c.sp-6]);
if(c.status)return c.status;goto P315;}
P315:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(19);
if(c.status)return c.status;goto P316;}
P316:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{324ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P319;}
P319:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P320;}
P320:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P322;}
P322:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P323;}
P323:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P324;}
P324:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P325;}
P325:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P326;}
P326:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
calldata_load();
if(c.status)return c.status;goto P327;}
P327:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P336;}
P336:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P337;}
P337:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(17);
if(c.status)return c.status;goto P338;}
P338:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P339;}
P339:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{347ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P342;}
P342:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P343;}
P343:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P345;}
P345:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P346;}
P346:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P347;}
P347:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P348;}
P348:{
c.op_raw=0;if(!c.charge(3)||!c.need(6,1))return c.status;
c.push(c.stack[c.sp-6]);
if(c.status)return c.status;goto P349;}
P349:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P351;}
P351:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P352;}
P352:{
c.op_raw=0;if(!c.charge(3)||!c.need(5,1))return c.status;
c.push(c.stack[c.sp-5]);
if(c.status)return c.status;goto P353;}
P353:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P354;}
P354:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P355;}
P355:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(17);
if(c.status)return c.status;goto P356;}
P356:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P357;}
P357:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{365ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P360;}
P360:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P361;}
P361:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P363;}
P363:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P364;}
P364:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P365;}
P365:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P366;}
P366:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P368;}
P368:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P369;}
P369:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P370;}
P370:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P371;}
P371:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P372;}
P372:{
c.op_raw=0;if(!c.charge(3)||!c.need(7,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-7];c.stack[c.sp-7]=t;}
if(c.status)return c.status;goto P373;}
P373:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P374;}
P374:{
c.op_raw=0;if(!c.charge(3)||!c.need(6,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-6];c.stack[c.sp-6]=t;}
if(c.status)return c.status;goto P375;}
P375:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P376;}
P376:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
if(c.status)return c.status;goto P377;}
P377:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P378;}
P378:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P379;}
P379:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P380;}
P380:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P381;}
P381:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P382;}
P382:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P384;}
P384:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P386;}
P386:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P387;}
P387:{
c.op_raw=0;if(!c.charge(3)||!c.need(5,1))return c.status;
c.push(c.stack[c.sp-5]);
if(c.status)return c.status;goto P388;}
P388:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P389;}
P389:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(18);
if(c.status)return c.status;goto P390;}
P390:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
unary(21);
if(c.status)return c.status;goto P391;}
P391:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{399ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P394;}
P394:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P395;}
P395:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P397;}
P397:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P398;}
P398:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P399;}
P399:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P400;}
P400:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P401;}
P401:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,0))return c.status;
calldata_load();
if(c.status)return c.status;goto P402;}
P402:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P404;}
P404:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P406;}
P406:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P408;}
P408:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(27);
if(c.status)return c.status;goto P409;}
P409:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(3);
if(c.status)return c.status;goto P410;}
P410:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P411;}
P411:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(22);
if(c.status)return c.status;goto P412;}
P412:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P413;}
P413:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(20);
if(c.status)return c.status;goto P414;}
P414:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{422ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P417;}
P417:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-2))return c.status;
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
if(c.status)return c.status;goto P418;}
P418:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P420;}
P420:{
c.op_raw=0;if(!c.charge(3)||!c.need(1,1))return c.status;
c.push(c.stack[c.sp-1]);
if(c.status)return c.status;goto P421;}
P421:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P422;}
P422:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P423;}
P423:{
c.op_raw=0;if(!c.charge(3)||!c.need(5,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-5];c.stack[c.sp-5]=t;}
if(c.status)return c.status;goto P424;}
P424:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
if(c.status)return c.status;goto P425;}
P425:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P426;}
P426:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P427;}
P427:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P428;}
P428:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P429;}
P429:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P430;}
P430:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{1313373041ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P435;}
P435:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P437;}
P437:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(27);
if(c.status)return c.status;goto P438;}
P438:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P440;}
P440:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P441;}
P441:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{50ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P443;}
P443:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P445;}
P445:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P446;}
P446:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{36ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P448;}
P448:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P450;}
P450:{
c.op_raw=0;if(!c.charge(0)||!c.need(2,-2))return c.status;
finish(REVERTED);return c.status;
if(c.status)return c.status;goto P451;}
P451:{
c.op_raw=0;if(!c.charge(1)||!c.need(0,0))return c.status;

if(c.status)return c.status;goto P452;}
P452:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P453;}
P453:{
c.op_raw=0;if(!c.charge(3)||!c.need(4,1))return c.status;
c.push(c.stack[c.sp-4]);
if(c.status)return c.status;goto P454;}
P454:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,1))return c.status;
c.push(c.stack[c.sp-3]);
if(c.status)return c.status;goto P455;}
P455:{
c.op_raw=0;if(!c.charge(0)||!c.need(3,-3))return c.status;
copy_op(55,CODE,480);
if(c.status)return c.status;goto P456;}
P456:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P458;}
P458:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P459;}
P459:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(1);
if(c.status)return c.status;goto P460;}
P460:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P461;}
P461:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,1))return c.status;
c.push(c.stack[c.sp-2]);
if(c.status)return c.status;goto P462;}
P462:{
c.op_raw=0;if(!c.charge(1)||!c.need(2,-2))return c.status;
memory_op(82);
if(c.status)return c.status;goto P463;}
P463:{
c.op_raw=0;if(!c.charge(3)||!c.need(3,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
if(c.status)return c.status;goto P464;}
P464:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,0))return c.status;
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
if(c.status)return c.status;goto P465;}
P465:{
c.op_raw=0;if(!c.charge(2)||!c.need(1,-1))return c.status;
c.pop();
if(c.status)return c.status;goto P466;}
P466:{
c.op_raw=0;if(!c.charge(8)||!c.need(1,-1))return c.status;
c.jump=c.pop();goto DISPATCH;
if(c.status)return c.status;goto P467;}
P467:{
c.op_raw=0;if(!c.charge(0)||!c.need(0,0))return c.status;
return INVALID;
if(c.status)return c.status;goto P468;}
P468:{
c.op_raw=0;if(!c.charge(375)||!c.need(3,-3))return c.status;
if(!c.charge(375))return c.status;log_op(1);
if(c.status)return c.status;goto P469;}
P469:{
c.op_raw=0;if(!c.charge(3)||!c.need(0,1))return c.status;
c.push(Word{{495790613315ULL,0ULL,0ULL,0ULL}});
if(c.status)return c.status;goto P475;}
P475:{
c.op_raw=0;if(!c.charge(0)||!c.need(0,0))return c.status;
return SUCCESS;
if(c.status)return c.status;goto P476;}
P476:{
c.op_raw=0;if(!c.charge(8)||!c.need(3,-2))return c.status;
ternary(8);
if(c.status)return c.status;goto P477;}
P477:{
c.op_raw=0;if(!c.charge(3)||!c.need(2,-1))return c.status;
binary(26);
if(c.status)return c.status;goto P478;}
P478:{
c.op_raw=0;if(!c.charge(0)||!c.need(0,0))return c.status;
return SUCCESS;
if(c.status)return c.status;goto P479;}
P479:{
c.op_raw=0;if(!c.charge(10)||!c.need(2,-1))return c.status;
binary(10);
if(c.status)return c.status;goto P480;}
P480: return SUCCESS;
DISPATCH: if(c.jump.high())return BAD_JUMP;switch(c.jump.v[0]){
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
