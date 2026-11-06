// SPDX-License-Identifier: LGPL-3.0-or-later
// Original implementation: no EF/CF/evm2cpp code is copied.
#pragma once
#include <stdint.h>
#include <stddef.h>
extern "C" {
__attribute__((import_module("host"),import_name("sload"))) void h_sload(uint32_t,uint32_t);
__attribute__((import_module("host"),import_name("sstore"))) uint64_t h_sstore(uint32_t,uint32_t);
__attribute__((import_module("host"),import_name("keccak"))) void h_keccak(uint32_t,uint32_t,uint32_t);
__attribute__((import_module("host"),import_name("log"))) void h_log(uint32_t,uint32_t,uint32_t,uint32_t);
}
struct Word {
 uint64_t v[4];
 static Word small(uint64_t n){return Word{{n,0,0,0}};}
 bool zero()const{return !(v[0]|v[1]|v[2]|v[3]);}
 bool high()const{return v[1]|v[2]|v[3];}
 bool negative()const{return v[3]>>63;}
 bool bit(unsigned n)const{return(v[n/64]>>(n%64))&1;}
 void bitset(unsigned n){v[n/64]|=uint64_t(1)<<(n%64);}
};
__attribute__((always_inline)) inline bool eq(Word a,Word b){return a.v[0]==b.v[0]&&a.v[1]==b.v[1]&&a.v[2]==b.v[2]&&a.v[3]==b.v[3];}
__attribute__((always_inline)) inline int cmp(Word a,Word b){for(int i=3;i>=0;--i)if(a.v[i]!=b.v[i])return a.v[i]>b.v[i]?1:-1;return 0;}
__attribute__((always_inline)) inline Word add(Word a,Word b){Word r{};uint64_t c=0;for(unsigned i=0;i<4;++i){uint64_t t=a.v[i]+b.v[i],k=t<a.v[i];r.v[i]=t+c;c=k|(r.v[i]<t);}return r;}
__attribute__((always_inline)) inline Word invert(Word a){for(auto&x:a.v)x=~x;return a;}
__attribute__((always_inline)) inline Word neg(Word a){return add(invert(a),Word::small(1));}
__attribute__((always_inline)) inline Word sub(Word a,Word b){return add(a,neg(b));}
__attribute__((always_inline)) inline Word mul(Word a,Word b){
 Word r{};for(unsigned i=0;i<8;++i){uint64_t carry=0;uint32_t av=uint32_t(a.v[i/2]>>(32*(i%2)));
 for(unsigned j=0;j+i<8;++j){unsigned k=i+j;uint32_t bv=uint32_t(b.v[j/2]>>(32*(j%2))),old=uint32_t(r.v[k/2]>>(32*(k%2)));
 uint64_t t=uint64_t(av)*bv+old+carry;carry=t>>32;uint64_t mask=uint64_t(0xffffffff)<<(32*(k%2));r.v[k/2]=(r.v[k/2]&~mask)|(uint64_t(uint32_t(t))<<(32*(k%2)));}}return r;
}
__attribute__((always_inline)) inline Word shl(Word a,uint64_t n){if(n>=256)return Word{};Word r{};unsigned w=n/64,b=n%64;for(int i=3;i>=int(w);--i){r.v[i]=a.v[i-w]<<b;if(b&&i>int(w))r.v[i]|=a.v[i-w-1]>>(64-b);}return r;}
__attribute__((always_inline)) inline Word shr(Word a,uint64_t n){if(n>=256)return Word{};Word r{};unsigned w=n/64,b=n%64;for(unsigned i=0;i+w<4;++i){r.v[i]=a.v[i+w]>>b;if(b&&i+w+1<4)r.v[i]|=a.v[i+w+1]<<(64-b);}return r;}
__attribute__((always_inline)) inline Word signed_shr(Word a,uint64_t n){Word r=shr(a,n);if(a.negative()){if(n>=256)return invert(Word{});Word mask=invert(shr(invert(Word{}),n));for(unsigned i=0;i<4;++i)r.v[i]|=mask.v[i];}return r;}
__attribute__((always_inline)) inline Word divide(Word a,Word b,bool rem=false){
 if(b.zero())return Word{};if(!a.high()&&!b.high())return Word::small(rem?a.v[0]%b.v[0]:a.v[0]/b.v[0]);
 if(cmp(a,b)<0)return rem?a:Word{};Word q{},r{};
 for(int i=255;i>=0;--i){bool overflow=r.negative();r=shl(r,1);r.v[0]|=a.bit(i);if(overflow||cmp(r,b)>=0){r=sub(r,b);q.bitset(i);}}return rem?r:q;
}
__attribute__((always_inline)) inline Word addmod(Word a,Word b,Word m){if(m.zero())return Word{};a=divide(a,m,true);b=divide(b,m,true);Word gap=sub(m,b);return cmp(a,gap)>=0?sub(a,gap):add(a,b);}
__attribute__((always_inline)) inline Word mulmod(Word a,Word b,Word m){if(m.zero())return Word{};a=divide(a,m,true);Word r{};for(unsigned i=0;i<256;++i){if(b.bit(i))r=addmod(r,a,m);a=addmod(a,a,m);}return r;}
__attribute__((always_inline)) inline Word from_be(const uint8_t*p){Word r{};for(unsigned i=0;i<32;++i)r.v[(31-i)/8]|=uint64_t(p[i])<<(8*((31-i)%8));return r;}
__attribute__((always_inline)) inline void to_be(Word a,uint8_t*p){for(unsigned i=0;i<32;++i)p[i]=uint8_t(a.v[(31-i)/8]>>(8*((31-i)%8)));}
__attribute__((always_inline)) inline uint32_t pointer(const void*p){return uint32_t(uintptr_t(p));}
enum Status:uint32_t{SUCCESS=0,REVERTED=1,OUT_OF_ENERGY=2,INVALID=3,STACK_UNDERFLOW=4,STACK_OVERFLOW=5,BAD_JUMP=6,EXCEPTION=7,UNSUPPORTED=8};
static constexpr uint32_t MEM_LIMIT=3*1024*1024;
struct Context {
 Word stack[1024];uint32_t sp=0;uint8_t mem[MEM_LIMIT];uint32_t mem_size=0;
 uint8_t input[65536];uint32_t input_len=0;uint64_t remaining=0,op_raw=0;int64_t factor=0;
 uint32_t status=0,output_start=0,output_len=0;Word jump{};
 void reset(uint32_t len,uint64_t limit,int64_t f){for(uint32_t i=0;i<mem_size;++i)mem[i]=0;sp=0;mem_size=0;input_len=len;remaining=limit;factor=f>0?f:0;status=0;output_start=0;output_len=0;}
 __attribute__((always_inline)) bool need(unsigned n,int delta){if(sp<n){status=STACK_UNDERFLOW;return false;}if(int(sp)+delta>1024){status=STACK_OVERFLOW;return false;}return true;}
 __attribute__((always_inline)) bool charge(uint64_t n){
 if(factor&&n){
  // The raw per-op costs here are bounded by 3 MiB memory and 256-bit operands.
  // Scale the opcode's aggregate cost once, preserving split-charge rounding.
  uint64_t f=uint64_t(factor),old=op_raw;
  if(n>UINT64_MAX-old){status=OUT_OF_ENERGY;return false;}
  op_raw+=n;
  if(f>UINT64_MAX/(op_raw?op_raw:1)){status=OUT_OF_ENERGY;return false;}
  volatile uint64_t decimal=10000;
  uint64_t penalty=op_raw*f/decimal-old*f/decimal;
  if(n>UINT64_MAX-penalty){status=OUT_OF_ENERGY;return false;}n+=penalty;
 }
 if(remaining<n){status=OUT_OF_ENERGY;remaining=0;return false;}remaining-=n;return true;
 }
 __attribute__((always_inline)) Word pop(){return stack[--sp];} __attribute__((always_inline))void push(Word a){stack[sp++]=a;}
 __attribute__((always_inline)) bool range(Word off,Word len,uint32_t&o,uint32_t&n){if(len.zero()){o=0;n=0;return true;}if(off.high()||len.high()||off.v[0]>MEM_LIMIT||len.v[0]>MEM_LIMIT-off.v[0]){status=OUT_OF_ENERGY;return false;}o=uint32_t(off.v[0]);n=uint32_t(len.v[0]);return true;}
 __attribute__((always_inline)) bool memory(uint32_t off,uint32_t n,uint64_t extra=0){uint64_t end=n?uint64_t(off)+n:0;if(end>MEM_LIMIT){status=OUT_OF_ENERGY;return false;}uint64_t old=mem_size/32,w=(end+31)/32;uint64_t d=w>old?(3*w+w*w/512)-(3*old+old*old/512):0;if(!charge(d+extra))return false;if(w>old)mem_size=uint32_t(w*32);return true;}
};
static Context c;
__attribute__((always_inline)) inline void binary(unsigned op){
 Word a=c.pop(),b=c.pop(),r{};
 switch(op){
 case 1:r=add(a,b);break;case 2:r=mul(a,b);break;case 3:r=sub(a,b);break;case 4:r=divide(a,b);break;
 case 5:{bool sign=a.negative()!=b.negative();r=divide(a.negative()?neg(a):a,b.negative()?neg(b):b);if(sign)r=neg(r);break;}
 case 6:r=divide(a,b,true);break;case 7:{bool sign=a.negative();r=divide(sign?neg(a):a,b.negative()?neg(b):b,true);if(sign)r=neg(r);break;}
 case 10:{unsigned bytes=0;for(int i=31;i>=0;--i)if(uint8_t(b.v[i/8]>>(8*(i%8)))){bytes=i+1;break;}if(!c.charge(10*bytes))return;r=Word::small(1);while(!b.zero()){if(b.bit(0))r=mul(r,a);b=shr(b,1);if(!b.zero())a=mul(a,a);}break;}
 case 11:{if(a.high()||a.v[0]>=32){r=b;break;}unsigned bit=unsigned(a.v[0])*8+7;r=b;for(unsigned i=bit+1;i<256;++i){uint64_t mask=uint64_t(1)<<(i%64);if(b.bit(bit))r.v[i/64]|=mask;else r.v[i/64]&=~mask;}break;}
 case 0x10:r=Word::small(cmp(a,b)<0);break;case 0x11:r=Word::small(cmp(a,b)>0);break;
 case 0x12:r=Word::small(a.negative()!=b.negative()?a.negative():cmp(a,b)<0);break;
 case 0x13:r=Word::small(a.negative()!=b.negative()?b.negative():cmp(a,b)>0);break;
 case 0x14:r=Word::small(eq(a,b));break;
 case 0x16:for(unsigned i=0;i<4;++i)r.v[i]=a.v[i]&b.v[i];break;
 case 0x17:for(unsigned i=0;i<4;++i)r.v[i]=a.v[i]|b.v[i];break;
 case 0x18:for(unsigned i=0;i<4;++i)r.v[i]=a.v[i]^b.v[i];break;
 case 0x1a:if(!a.high()&&a.v[0]<32){unsigned i=31-unsigned(a.v[0]);r=Word::small(uint8_t(b.v[i/8]>>(8*(i%8))));}break;
 case 0x1b:r=shl(b,a.high()?256:a.v[0]);break;case 0x1c:r=shr(b,a.high()?256:a.v[0]);break;case 0x1d:r=signed_shr(b,a.high()?256:a.v[0]);break;
 }c.push(r);
}
__attribute__((always_inline)) inline void ternary(unsigned op){Word a=c.pop(),b=c.pop(),m=c.pop();c.push(op==8?addmod(a,b,m):mulmod(a,b,m));}
__attribute__((always_inline)) inline void unary(unsigned op){Word a=c.pop();c.push(op==0x15?Word::small(a.zero()):invert(a));}
__attribute__((always_inline)) inline void memory_op(unsigned op){Word offset=c.pop();uint32_t o,n;if(!c.range(offset,Word::small(op==0x53?1:32),o,n)||!c.memory(o,n))return;if(op==0x51)c.push(from_be(c.mem+o));else{Word value=c.pop();if(op==0x52)to_be(value,c.mem+o);else c.mem[o]=uint8_t(value.v[0]);}}
__attribute__((always_inline)) inline void copy_op(unsigned op,const uint8_t*code,uint32_t code_len){
 Word dst=c.pop(),src=c.pop(),length=c.pop();uint32_t o,n;if(!c.range(dst,length,o,n))return;
 if(op==0x3e&&(src.high()||src.v[0]>0||n>0)){c.status=EXCEPTION;return;}
 if(!c.memory(o,n,3*((uint64_t(n)+31)/32)))return;const uint8_t*data=op==0x39?code:c.input;uint32_t len=op==0x39?code_len:c.input_len;
 for(uint32_t i=0;i<n;++i)c.mem[o+i]=(!src.high()&&src.v[0]<len&&i<len-src.v[0])?data[src.v[0]+i]:0;
}
__attribute__((always_inline)) inline void calldata_load(){Word off=c.pop();uint8_t b[32]{};if(!off.high()&&off.v[0]<c.input_len)for(unsigned i=0;i<32&&off.v[0]+i<c.input_len;++i)b[i]=c.input[off.v[0]+i];c.push(from_be(b));}
__attribute__((always_inline)) inline void hash_op(){Word off=c.pop(),len=c.pop();uint32_t o,n;if(!c.range(off,len,o,n)||!c.memory(o,n,6*((uint64_t(n)+31)/32)))return;Word out{};h_keccak(pointer(c.mem+o),n,pointer(&out));c.push(out);}
__attribute__((always_inline)) inline void storage_op(unsigned op){Word k=c.pop();if(op==0x54){Word v{};h_sload(pointer(&k),pointer(&v));c.push(v);}else{Word v=c.pop();c.charge(h_sstore(pointer(&k),pointer(&v)));}}
__attribute__((always_inline)) inline void log_op(unsigned t){Word off=c.pop(),len=c.pop();uint32_t o,n;if(!c.range(off,len,o,n)||!c.memory(o,n,uint64_t(n)*8))return;Word topics[4]{};for(unsigned i=0;i<t;++i)topics[i]=c.pop();h_log(pointer(c.mem+o),n,pointer(topics),t);}
__attribute__((always_inline)) inline void finish(unsigned status){Word off=c.pop(),len=c.pop();uint32_t o,n;if(!c.range(off,len,o,n)||!c.memory(o,n))return;c.output_start=o;c.output_len=n;c.status=status;}
extern "C" {
__attribute__((visibility("default"))) uint32_t input_ptr(){return pointer(c.input);}
__attribute__((visibility("default"))) uint64_t remaining_energy(){return c.remaining;}
__attribute__((visibility("default"))) uint32_t output_ptr(){return pointer(c.mem+c.output_start);}
__attribute__((visibility("default"))) uint32_t output_len(){return c.output_len;}
void* memset(void*d,int x,size_t n){auto*p=(uint8_t*)d;for(size_t i=0;i<n;++i)p[i]=(uint8_t)x;return d;}
void* memcpy(void*d,const void*s,size_t n){auto*p=(uint8_t*)d;auto*q=(const uint8_t*)s;for(size_t i=0;i<n;++i)p[i]=q[i];return d;}
void* memmove(void*d,const void*s,size_t n){auto*p=(uint8_t*)d;auto*q=(const uint8_t*)s;if(p<q)for(size_t i=0;i<n;++i)p[i]=q[i];else for(size_t i=n;i>0;--i)p[i-1]=q[i-1];return d;}
}

// Freestanding wasm32 compiler support, original implementation.
// optnone prevents LLVM recognizing this as another call to __multi3.
extern "C" __attribute__((optnone,noinline))
__uint128_t __multi3(__uint128_t a,__uint128_t b){
 union Wide{__uint128_t all;uint32_t limb[4];};
 Wide x,y,z;x.all=a;y.all=b;z.all=0;
 for(unsigned i=0;i<4;++i){uint64_t carry=0;
  for(unsigned j=0;j+i<4;++j){unsigned k=i+j;
   uint64_t t=uint64_t(x.limb[i])*y.limb[j]+z.limb[k]+carry;
   z.limb[k]=uint32_t(t);carry=t>>32;
  }
 }return z.all;
}


// V3 successful path: inline preflight, then one debit for the entire static region.
// Preserve per-op dynamic-factor rounding with a compile-time cost histogram.
struct BlockStep{uint64_t cost;unsigned required;int delta;};
struct CostCount{uint64_t cost,count;};
template<size_t N> __attribute__((always_inline)) inline
bool reserve_block(uint64_t raw,unsigned required,unsigned peak,const CostCount(&costs)[N]){
 if(peak>1024||c.sp<required||c.sp>1024-peak)return false;
 uint64_t total=raw;
 if(c.factor){
  for(const auto& entry:costs){
   const uint64_t penalty=(entry.cost*uint64_t(c.factor))/10000;
   total+=entry.count*penalty;
  }
 }
 if(c.remaining<total)return false;
 c.remaining-=total;
 return true;
}
// Only pure, static-cost instructions are grouped. If preflight fails, one of
// their resource checks must fail before the terminal control-flow instruction.
// Replay just those checks, in opcode order, to retain exact failure precedence.
// No word computation, external effect or VM instruction is executed speculatively.
__attribute__((cold,noinline)) inline uint32_t block_fault(const BlockStep*steps,size_t count){
 for(size_t i=0;i<count;++i){
  c.op_raw=0;
  if(!c.charge(steps[i].cost)||!c.need(steps[i].required,steps[i].delta))return c.status;
  c.sp=uint32_t(int(c.sp)+steps[i].delta);
 }
 c.status=EXCEPTION;return c.status; // An internal planner invariant failed.
}
