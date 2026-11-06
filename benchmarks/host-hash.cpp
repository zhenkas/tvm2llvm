// SPDX-License-Identifier: LGPL-3.0-or-later
// Diagnostic kernel only: repeated imports, no TVM instruction/energy accounting.
#include <stdint.h>
extern "C" __attribute__((import_module("host"),import_name("keccak")))
void host_keccak(uint32_t,uint32_t,uint32_t);
static uint8_t input[4], source[64], output[32];
static uint64_t remaining;
extern "C" {
uint32_t input_ptr(){return (uint32_t)(uintptr_t)input;}
uint32_t output_ptr(){return (uint32_t)(uintptr_t)output;}
uint32_t output_len(){return 32;}
uint64_t remaining_energy(){return remaining;}
uint32_t execute(uint32_t,uint64_t energy,int64_t){
 remaining=energy;
 for(unsigned i=0;i<32;++i)output[i]=0;
 uint32_t n=uint32_t(input[0])|(uint32_t(input[1])<<8)|(uint32_t(input[2])<<16)|(uint32_t(input[3])<<24);
 for(uint32_t i=0;i<n;++i)host_keccak((uint32_t)(uintptr_t)source,64,(uint32_t)(uintptr_t)output);
 return 0;
}}
