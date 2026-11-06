// SPDX-License-Identifier: LGPL-3.0-or-later
// Empty-import control, same parameter arity as the hash import.
#include <stdint.h>
extern "C" __attribute__((import_module("host"),import_name("noop")))
void host_noop(uint32_t,uint32_t,uint32_t);
extern "C" void run(uint32_t n){for(uint32_t i=0;i<n;++i)host_noop(0,64,0);}
