/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <types.h>

#ifdef TARGET_PLATFORM_i386

void asm_wrmsr(uint64_t msr, uint64_t val) {
    uint32_t eax, edx;

    // val has to be split into edx:eax
    edx = val >> 32;
    eax = val & 0xFFFFFFFF;

    asm volatile("wrmsr" : : "c"(msr), "a"(eax), "d"(edx));

    return;
}

#endif