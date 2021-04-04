/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>

#ifdef TARGET_PLATFORM_x86_64

void asm_wrmsr(uint64_t reg, uint64_t val) {
    uint32_t eax, edx;

    // val has to be split into edx:eax
    edx = val >> 32;
    eax = val & 0xFFFFFFFF;

    asm volatile("wrmsr" : : "c"(reg), "a"(eax), "d"(edx));

    return;
}

uint64_t asm_rdmsr(uint64_t reg) {
    uint32_t eax, edx;

    asm volatile("rdmsr" : "=a"(eax), "=d"(edx) : "c"(reg));

    // reads into edx:eax, so we have to combine them
    return (((uint64_t)edx) << 32) | eax;
}

#endif