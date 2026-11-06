#include "runtime.hpp"
static const uint8_t CODE[]={96,128,96,64,82,52,128,21,97,0,16,87,96,0,128,253,91,80,96,4,54,16,97,0,65,87,96,0,53,96,224,28,128,99,115,19,242,171,20,97,0,70,87,128,99,143,144,135,245,20,97,0,107,87,128,99,186,101,54,118,20,97,0,126,87,91,96,0,128,253,91,97,0,89,97,0,84,54,96,4,97,0,240,86,91,97,0,167,86,91,96,64,81,144,129,82,96,32,1,96,64,81,128,145,3,144,243,91,97,0,89,97,0,121,54,96,4,97,1,9,86,91,97,0,206,86,91,97,0,89,97,0,140,54,96,4,97,1,125,86,91,96,1,96,1,96,160,27,3,22,96,0,144,129,82,96,32,129,144,82,96,64,144,32,84,144,86,91,96,0,96,1,130,129,84,129,16,97,0,188,87,97,0,188,97,1,173,86,91,144,96,0,82,96,32,96,0,32,1,84,144,80,145,144,80,86,91,96,0,130,130,96,64,81,97,0,224,146,145,144,97,1,195,86,91,96,64,81,128,145,3,144,32,144,80,146,145,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,2,87,96,0,128,253,91,80,53,145,144,80,86,91,96,0,128,96,32,131,133,3,18,21,97,1,28,87,96,0,128,253,91,130,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,51,87,96,0,128,253,91,131,1,96,31,129,1,133,19,97,1,68,87,96,0,128,253,91,128,53,103,255,255,255,255,255,255,255,255,129,17,21,97,1,91,87,96,0,128,253,91,133,96,32,130,132,1,1,17,21,97,1,109,87,96,0,128,253,91,96,32,145,144,145,1,149,144,148,80,146,80,80,80,86,91,96,0,96,32,130,132,3,18,21,97,1,143,87,96,0,128,253,91,129,53,96,1,96,1,96,160,27,3,129,22,129,20,97,1,166,87,96,0,128,253,91,147,146,80,80,80,86,91,99,78,72,123,113,96,224,27,96,0,82,96,50,96,4,82,96,36,96,0,253,91,129,131,130,55,96,0,145,1,144,129,82,145,144,80,86,254,161,100,115,111,108,99,67,0,8,26,0,10,};
extern "C" __attribute__((visibility("default"))) uint32_t execute(uint32_t len,uint64_t energy,int64_t factor){c.reset(len,energy,factor);if(len>65536)return UNSUPPORTED;try{goto P0;
P0:{
reserve<96>(3, 0, 1);
c.push(Word{{128ULL,0ULL,0ULL,0ULL}});
goto P2;}
P2:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P4;}
P4:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P5;}
P5:{
reserve<52>(2, 0, 1);
c.push(Word{});
goto P6;}
P6:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P7;}
P7:{
reserve<21>(3, 1, 0);
unary<21>();
goto P8;}
P8:{
reserve<97>(3, 0, 1);
c.push(Word{{16ULL,0ULL,0ULL,0ULL}});
goto P11;}
P11:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P12;}
P12:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P14;}
P14:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P15;}
P15:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P16;}
P16:{
reserve<91>(1, 0, 0);

goto P17;}
P17:{
reserve<80>(2, 1, -1);
c.pop();
goto P18;}
P18:{
reserve<96>(3, 0, 1);
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P20;}
P20:{
reserve<54>(2, 0, 1);
c.push(Word::small(c.input_len));
goto P21;}
P21:{
reserve<16>(3, 2, -1);
binary<16>();
goto P22;}
P22:{
reserve<97>(3, 0, 1);
c.push(Word{{65ULL,0ULL,0ULL,0ULL}});
goto P25;}
P25:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P26;}
P26:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P28;}
P28:{
reserve<53>(3, 1, 0);
calldata_load();
goto P29;}
P29:{
reserve<96>(3, 0, 1);
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
goto P31;}
P31:{
reserve<28>(3, 2, -1);
binary<28>();
goto P32;}
P32:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P33;}
P33:{
reserve<99>(3, 0, 1);
c.push(Word{{1930687147ULL,0ULL,0ULL,0ULL}});
goto P38;}
P38:{
reserve<20>(3, 2, -1);
binary<20>();
goto P39;}
P39:{
reserve<97>(3, 0, 1);
c.push(Word{{70ULL,0ULL,0ULL,0ULL}});
goto P42;}
P42:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P43;}
P43:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P44;}
P44:{
reserve<99>(3, 0, 1);
c.push(Word{{2408613877ULL,0ULL,0ULL,0ULL}});
goto P49;}
P49:{
reserve<20>(3, 2, -1);
binary<20>();
goto P50;}
P50:{
reserve<97>(3, 0, 1);
c.push(Word{{107ULL,0ULL,0ULL,0ULL}});
goto P53;}
P53:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P54;}
P54:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P55;}
P55:{
reserve<99>(3, 0, 1);
c.push(Word{{3127195254ULL,0ULL,0ULL,0ULL}});
goto P60;}
P60:{
reserve<20>(3, 2, -1);
binary<20>();
goto P61;}
P61:{
reserve<97>(3, 0, 1);
c.push(Word{{126ULL,0ULL,0ULL,0ULL}});
goto P64;}
P64:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P65;}
P65:{
reserve<91>(1, 0, 0);

goto P66;}
P66:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P68;}
P68:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P69;}
P69:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P70;}
P70:{
reserve<91>(1, 0, 0);

goto P71;}
P71:{
reserve<97>(3, 0, 1);
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
goto P74;}
P74:{
reserve<97>(3, 0, 1);
c.push(Word{{84ULL,0ULL,0ULL,0ULL}});
goto P77;}
P77:{
reserve<54>(2, 0, 1);
c.push(Word::small(c.input_len));
goto P78;}
P78:{
reserve<96>(3, 0, 1);
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P80;}
P80:{
reserve<97>(3, 0, 1);
c.push(Word{{240ULL,0ULL,0ULL,0ULL}});
goto P83;}
P83:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P84;}
P84:{
reserve<91>(1, 0, 0);

goto P85;}
P85:{
reserve<97>(3, 0, 1);
c.push(Word{{167ULL,0ULL,0ULL,0ULL}});
goto P88;}
P88:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P89;}
P89:{
reserve<91>(1, 0, 0);

goto P90;}
P90:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P92;}
P92:{
reserve<81>(1, 1, 0);
memory_op<81>();
goto P93;}
P93:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P94;}
P94:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P95;}
P95:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P96;}
P96:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P98;}
P98:{
reserve<1>(3, 2, -1);
binary<1>();
goto P99;}
P99:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P101;}
P101:{
reserve<81>(1, 1, 0);
memory_op<81>();
goto P102;}
P102:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P103;}
P103:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P104;}
P104:{
reserve<3>(3, 2, -1);
binary<3>();
goto P105;}
P105:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P106;}
P106:{
reserve<243>(0, 2, -2);
finish(SUCCESS);return c.status;
goto P107;}
P107:{
reserve<91>(1, 0, 0);

goto P108;}
P108:{
reserve<97>(3, 0, 1);
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
goto P111;}
P111:{
reserve<97>(3, 0, 1);
c.push(Word{{121ULL,0ULL,0ULL,0ULL}});
goto P114;}
P114:{
reserve<54>(2, 0, 1);
c.push(Word::small(c.input_len));
goto P115;}
P115:{
reserve<96>(3, 0, 1);
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P117;}
P117:{
reserve<97>(3, 0, 1);
c.push(Word{{265ULL,0ULL,0ULL,0ULL}});
goto P120;}
P120:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P121;}
P121:{
reserve<91>(1, 0, 0);

goto P122;}
P122:{
reserve<97>(3, 0, 1);
c.push(Word{{206ULL,0ULL,0ULL,0ULL}});
goto P125;}
P125:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P126;}
P126:{
reserve<91>(1, 0, 0);

goto P127;}
P127:{
reserve<97>(3, 0, 1);
c.push(Word{{89ULL,0ULL,0ULL,0ULL}});
goto P130;}
P130:{
reserve<97>(3, 0, 1);
c.push(Word{{140ULL,0ULL,0ULL,0ULL}});
goto P133;}
P133:{
reserve<54>(2, 0, 1);
c.push(Word::small(c.input_len));
goto P134;}
P134:{
reserve<96>(3, 0, 1);
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P136;}
P136:{
reserve<97>(3, 0, 1);
c.push(Word{{381ULL,0ULL,0ULL,0ULL}});
goto P139;}
P139:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P140;}
P140:{
reserve<91>(1, 0, 0);

goto P141;}
P141:{
reserve<96>(3, 0, 1);
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
goto P143;}
P143:{
reserve<96>(3, 0, 1);
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
goto P145;}
P145:{
reserve<96>(3, 0, 1);
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
goto P147;}
P147:{
reserve<27>(3, 2, -1);
binary<27>();
goto P148;}
P148:{
reserve<3>(3, 2, -1);
binary<3>();
goto P149;}
P149:{
reserve<22>(3, 2, -1);
binary<22>();
goto P150;}
P150:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P152;}
P152:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P153;}
P153:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P154;}
P154:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P155;}
P155:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P157;}
P157:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P158;}
P158:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P159;}
P159:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P160;}
P160:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P162;}
P162:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P163;}
P163:{
reserve<32>(30, 2, -1);
hash_op();
goto P164;}
P164:{
reserve<84>(50, 1, 0);
storage_op<84>();
goto P165;}
P165:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P166;}
P166:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P167;}
P167:{
reserve<91>(1, 0, 0);

goto P168;}
P168:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P170;}
P170:{
reserve<96>(3, 0, 1);
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
goto P172;}
P172:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P173;}
P173:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P174;}
P174:{
reserve<84>(50, 1, 0);
storage_op<84>();
goto P175;}
P175:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P176;}
P176:{
reserve<16>(3, 2, -1);
binary<16>();
goto P177;}
P177:{
reserve<97>(3, 0, 1);
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
goto P180;}
P180:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P181;}
P181:{
reserve<97>(3, 0, 1);
c.push(Word{{188ULL,0ULL,0ULL,0ULL}});
goto P184;}
P184:{
reserve<97>(3, 0, 1);
c.push(Word{{429ULL,0ULL,0ULL,0ULL}});
goto P187;}
P187:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P188;}
P188:{
reserve<91>(1, 0, 0);

goto P189;}
P189:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P190;}
P190:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P192;}
P192:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P193;}
P193:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P195;}
P195:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P197;}
P197:{
reserve<32>(30, 2, -1);
hash_op();
goto P198;}
P198:{
reserve<1>(3, 2, -1);
binary<1>();
goto P199;}
P199:{
reserve<84>(50, 1, 0);
storage_op<84>();
goto P200;}
P200:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P201;}
P201:{
reserve<80>(2, 1, -1);
c.pop();
goto P202;}
P202:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P203;}
P203:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P204;}
P204:{
reserve<80>(2, 1, -1);
c.pop();
goto P205;}
P205:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P206;}
P206:{
reserve<91>(1, 0, 0);

goto P207;}
P207:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P209;}
P209:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P210;}
P210:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P211;}
P211:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P213;}
P213:{
reserve<81>(1, 1, 0);
memory_op<81>();
goto P214;}
P214:{
reserve<97>(3, 0, 1);
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
goto P217;}
P217:{
reserve<146>(3, 4, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
goto P218;}
P218:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P219;}
P219:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P220;}
P220:{
reserve<97>(3, 0, 1);
c.push(Word{{451ULL,0ULL,0ULL,0ULL}});
goto P223;}
P223:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P224;}
P224:{
reserve<91>(1, 0, 0);

goto P225;}
P225:{
reserve<96>(3, 0, 1);
c.push(Word{{64ULL,0ULL,0ULL,0ULL}});
goto P227;}
P227:{
reserve<81>(1, 1, 0);
memory_op<81>();
goto P228;}
P228:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P229;}
P229:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P230;}
P230:{
reserve<3>(3, 2, -1);
binary<3>();
goto P231;}
P231:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P232;}
P232:{
reserve<32>(30, 2, -1);
hash_op();
goto P233;}
P233:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P234;}
P234:{
reserve<80>(2, 1, -1);
c.pop();
goto P235;}
P235:{
reserve<146>(3, 4, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
goto P236;}
P236:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P237;}
P237:{
reserve<80>(2, 1, -1);
c.pop();
goto P238;}
P238:{
reserve<80>(2, 1, -1);
c.pop();
goto P239;}
P239:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P240;}
P240:{
reserve<91>(1, 0, 0);

goto P241;}
P241:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P243;}
P243:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P245;}
P245:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P246;}
P246:{
reserve<132>(3, 5, 1);
c.push(c.stack[c.sp-5]);
goto P247;}
P247:{
reserve<3>(3, 2, -1);
binary<3>();
goto P248;}
P248:{
reserve<18>(3, 2, -1);
binary<18>();
goto P249;}
P249:{
reserve<21>(3, 1, 0);
unary<21>();
goto P250;}
P250:{
reserve<97>(3, 0, 1);
c.push(Word{{258ULL,0ULL,0ULL,0ULL}});
goto P253;}
P253:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P254;}
P254:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P256;}
P256:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P257;}
P257:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P258;}
P258:{
reserve<91>(1, 0, 0);

goto P259;}
P259:{
reserve<80>(2, 1, -1);
c.pop();
goto P260;}
P260:{
reserve<53>(3, 1, 0);
calldata_load();
goto P261;}
P261:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P262;}
P262:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P263;}
P263:{
reserve<80>(2, 1, -1);
c.pop();
goto P264;}
P264:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P265;}
P265:{
reserve<91>(1, 0, 0);

goto P266;}
P266:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P268;}
P268:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P269;}
P269:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P271;}
P271:{
reserve<131>(3, 4, 1);
c.push(c.stack[c.sp-4]);
goto P272;}
P272:{
reserve<133>(3, 6, 1);
c.push(c.stack[c.sp-6]);
goto P273;}
P273:{
reserve<3>(3, 2, -1);
binary<3>();
goto P274;}
P274:{
reserve<18>(3, 2, -1);
binary<18>();
goto P275;}
P275:{
reserve<21>(3, 1, 0);
unary<21>();
goto P276;}
P276:{
reserve<97>(3, 0, 1);
c.push(Word{{284ULL,0ULL,0ULL,0ULL}});
goto P279;}
P279:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P280;}
P280:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P282;}
P282:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P283;}
P283:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P284;}
P284:{
reserve<91>(1, 0, 0);

goto P285;}
P285:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P286;}
P286:{
reserve<53>(3, 1, 0);
calldata_load();
goto P287;}
P287:{
reserve<103>(3, 0, 1);
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
goto P296;}
P296:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P297;}
P297:{
reserve<17>(3, 2, -1);
binary<17>();
goto P298;}
P298:{
reserve<21>(3, 1, 0);
unary<21>();
goto P299;}
P299:{
reserve<97>(3, 0, 1);
c.push(Word{{307ULL,0ULL,0ULL,0ULL}});
goto P302;}
P302:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P303;}
P303:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P305;}
P305:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P306;}
P306:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P307;}
P307:{
reserve<91>(1, 0, 0);

goto P308;}
P308:{
reserve<131>(3, 4, 1);
c.push(c.stack[c.sp-4]);
goto P309;}
P309:{
reserve<1>(3, 2, -1);
binary<1>();
goto P310;}
P310:{
reserve<96>(3, 0, 1);
c.push(Word{{31ULL,0ULL,0ULL,0ULL}});
goto P312;}
P312:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P313;}
P313:{
reserve<1>(3, 2, -1);
binary<1>();
goto P314;}
P314:{
reserve<133>(3, 6, 1);
c.push(c.stack[c.sp-6]);
goto P315;}
P315:{
reserve<19>(3, 2, -1);
binary<19>();
goto P316;}
P316:{
reserve<97>(3, 0, 1);
c.push(Word{{324ULL,0ULL,0ULL,0ULL}});
goto P319;}
P319:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P320;}
P320:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P322;}
P322:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P323;}
P323:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P324;}
P324:{
reserve<91>(1, 0, 0);

goto P325;}
P325:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P326;}
P326:{
reserve<53>(3, 1, 0);
calldata_load();
goto P327;}
P327:{
reserve<103>(3, 0, 1);
c.push(Word{{18446744073709551615ULL,0ULL,0ULL,0ULL}});
goto P336;}
P336:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P337;}
P337:{
reserve<17>(3, 2, -1);
binary<17>();
goto P338;}
P338:{
reserve<21>(3, 1, 0);
unary<21>();
goto P339;}
P339:{
reserve<97>(3, 0, 1);
c.push(Word{{347ULL,0ULL,0ULL,0ULL}});
goto P342;}
P342:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P343;}
P343:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P345;}
P345:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P346;}
P346:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P347;}
P347:{
reserve<91>(1, 0, 0);

goto P348;}
P348:{
reserve<133>(3, 6, 1);
c.push(c.stack[c.sp-6]);
goto P349;}
P349:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P351;}
P351:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P352;}
P352:{
reserve<132>(3, 5, 1);
c.push(c.stack[c.sp-5]);
goto P353;}
P353:{
reserve<1>(3, 2, -1);
binary<1>();
goto P354;}
P354:{
reserve<1>(3, 2, -1);
binary<1>();
goto P355;}
P355:{
reserve<17>(3, 2, -1);
binary<17>();
goto P356;}
P356:{
reserve<21>(3, 1, 0);
unary<21>();
goto P357;}
P357:{
reserve<97>(3, 0, 1);
c.push(Word{{365ULL,0ULL,0ULL,0ULL}});
goto P360;}
P360:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P361;}
P361:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P363;}
P363:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P364;}
P364:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P365;}
P365:{
reserve<91>(1, 0, 0);

goto P366;}
P366:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P368;}
P368:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P369;}
P369:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P370;}
P370:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P371;}
P371:{
reserve<1>(3, 2, -1);
binary<1>();
goto P372;}
P372:{
reserve<149>(3, 7, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-7];c.stack[c.sp-7]=t;}
goto P373;}
P373:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P374;}
P374:{
reserve<148>(3, 6, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-6];c.stack[c.sp-6]=t;}
goto P375;}
P375:{
reserve<80>(2, 1, -1);
c.pop();
goto P376;}
P376:{
reserve<146>(3, 4, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
goto P377;}
P377:{
reserve<80>(2, 1, -1);
c.pop();
goto P378;}
P378:{
reserve<80>(2, 1, -1);
c.pop();
goto P379;}
P379:{
reserve<80>(2, 1, -1);
c.pop();
goto P380;}
P380:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P381;}
P381:{
reserve<91>(1, 0, 0);

goto P382;}
P382:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P384;}
P384:{
reserve<96>(3, 0, 1);
c.push(Word{{32ULL,0ULL,0ULL,0ULL}});
goto P386;}
P386:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P387;}
P387:{
reserve<132>(3, 5, 1);
c.push(c.stack[c.sp-5]);
goto P388;}
P388:{
reserve<3>(3, 2, -1);
binary<3>();
goto P389;}
P389:{
reserve<18>(3, 2, -1);
binary<18>();
goto P390;}
P390:{
reserve<21>(3, 1, 0);
unary<21>();
goto P391;}
P391:{
reserve<97>(3, 0, 1);
c.push(Word{{399ULL,0ULL,0ULL,0ULL}});
goto P394;}
P394:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P395;}
P395:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P397;}
P397:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P398;}
P398:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P399;}
P399:{
reserve<91>(1, 0, 0);

goto P400;}
P400:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P401;}
P401:{
reserve<53>(3, 1, 0);
calldata_load();
goto P402;}
P402:{
reserve<96>(3, 0, 1);
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
goto P404;}
P404:{
reserve<96>(3, 0, 1);
c.push(Word{{1ULL,0ULL,0ULL,0ULL}});
goto P406;}
P406:{
reserve<96>(3, 0, 1);
c.push(Word{{160ULL,0ULL,0ULL,0ULL}});
goto P408;}
P408:{
reserve<27>(3, 2, -1);
binary<27>();
goto P409;}
P409:{
reserve<3>(3, 2, -1);
binary<3>();
goto P410;}
P410:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P411;}
P411:{
reserve<22>(3, 2, -1);
binary<22>();
goto P412;}
P412:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P413;}
P413:{
reserve<20>(3, 2, -1);
binary<20>();
goto P414;}
P414:{
reserve<97>(3, 0, 1);
c.push(Word{{422ULL,0ULL,0ULL,0ULL}});
goto P417;}
P417:{
reserve<87>(10, 2, -2);
c.jump=c.pop();if(!c.pop().zero())goto DISPATCH;
goto P418;}
P418:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P420;}
P420:{
reserve<128>(3, 1, 1);
c.push(c.stack[c.sp-1]);
goto P421;}
P421:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P422;}
P422:{
reserve<91>(1, 0, 0);

goto P423;}
P423:{
reserve<147>(3, 5, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-5];c.stack[c.sp-5]=t;}
goto P424;}
P424:{
reserve<146>(3, 4, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-4];c.stack[c.sp-4]=t;}
goto P425;}
P425:{
reserve<80>(2, 1, -1);
c.pop();
goto P426;}
P426:{
reserve<80>(2, 1, -1);
c.pop();
goto P427;}
P427:{
reserve<80>(2, 1, -1);
c.pop();
goto P428;}
P428:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P429;}
P429:{
reserve<91>(1, 0, 0);

goto P430;}
P430:{
reserve<99>(3, 0, 1);
c.push(Word{{1313373041ULL,0ULL,0ULL,0ULL}});
goto P435;}
P435:{
reserve<96>(3, 0, 1);
c.push(Word{{224ULL,0ULL,0ULL,0ULL}});
goto P437;}
P437:{
reserve<27>(3, 2, -1);
binary<27>();
goto P438;}
P438:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P440;}
P440:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P441;}
P441:{
reserve<96>(3, 0, 1);
c.push(Word{{50ULL,0ULL,0ULL,0ULL}});
goto P443;}
P443:{
reserve<96>(3, 0, 1);
c.push(Word{{4ULL,0ULL,0ULL,0ULL}});
goto P445;}
P445:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P446;}
P446:{
reserve<96>(3, 0, 1);
c.push(Word{{36ULL,0ULL,0ULL,0ULL}});
goto P448;}
P448:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P450;}
P450:{
reserve<253>(0, 2, -2);
finish(REVERTED);return c.status;
goto P451;}
P451:{
reserve<91>(1, 0, 0);

goto P452;}
P452:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P453;}
P453:{
reserve<131>(3, 4, 1);
c.push(c.stack[c.sp-4]);
goto P454;}
P454:{
reserve<130>(3, 3, 1);
c.push(c.stack[c.sp-3]);
goto P455;}
P455:{
reserve<55>(0, 3, -3);
copy_op<55>(CODE,480);
goto P456;}
P456:{
reserve<96>(3, 0, 1);
c.push(Word{{0ULL,0ULL,0ULL,0ULL}});
goto P458;}
P458:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P459;}
P459:{
reserve<1>(3, 2, -1);
binary<1>();
goto P460;}
P460:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P461;}
P461:{
reserve<129>(3, 2, 1);
c.push(c.stack[c.sp-2]);
goto P462;}
P462:{
reserve<82>(1, 2, -2);
memory_op<82>();
goto P463;}
P463:{
reserve<145>(3, 3, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-3];c.stack[c.sp-3]=t;}
goto P464;}
P464:{
reserve<144>(3, 2, 0);
{Word t=c.stack[c.sp-1];c.stack[c.sp-1]=c.stack[c.sp-2];c.stack[c.sp-2]=t;}
goto P465;}
P465:{
reserve<80>(2, 1, -1);
c.pop();
goto P466;}
P466:{
reserve<86>(8, 1, -1);
c.jump=c.pop();goto DISPATCH;
goto P467;}
P467:{
reserve<254>(0, 0, 0);
return INVALID;
goto P468;}
P468:{
reserve<161>(375, 3, -3);
c.charge(375);log_op<1>();
goto P469;}
P469:{
reserve<100>(3, 0, 1);
c.push(Word{{495790613315ULL,0ULL,0ULL,0ULL}});
goto P475;}
P475:{
reserve<0>(0, 0, 0);
return SUCCESS;
goto P476;}
P476:{
reserve<8>(8, 3, -2);
ternary<8>();
goto P477;}
P477:{
reserve<26>(3, 2, -1);
binary<26>();
goto P478;}
P478:{
reserve<0>(0, 0, 0);
return SUCCESS;
goto P479;}
P479:{
reserve<10>(10, 2, -1);
binary<10>();
goto P480;}
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
default:return BAD_JUMP;}}catch(const VmExit& e){++caught;return e.status;}}
