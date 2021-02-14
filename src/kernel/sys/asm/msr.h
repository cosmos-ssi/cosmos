/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MSR_H
#define _MSR_H

#include <types.h>

#define MSR_EFER 0xC0000080    // EFER register
#define MSR_STAR 0xC0000081    // Segment selectors for SYSCALL/SYSRET
#define MSR_LSTAR 0xC0000082   // 64 bit RIP for kernel entry
#define MSR_CSTAR 0xC0000083   // Compatibility-mode RIP
#define MSR_SFMASK 0xC0000084  // RFLAGS mask for SYSCALL (this is a mask, so corresponding bits are cleared)

uint64_t asm_rdmsr(uint64_t reg);
void asm_wrmsr(uint64_t reg, uint64_t val);

#endif