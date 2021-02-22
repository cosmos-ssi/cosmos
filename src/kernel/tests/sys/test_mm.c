//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/mm/pagetables.h>
#include <tests/sys/test_mm.h>

void test_mm_stresstest() {
    uint32_t i = 1;
    uint8_t* buffer = kmalloc(1024 * 1024);
    while (0 != buffer) {
        buffer = kmalloc(1024 * 1024);
        i += 1;
        kprintf("i, addr: %llu, 0x%llX\n", i, buffer);
    }
}

void test_mm() {
    kprintf("Testing mm\n");
    test_mm_stresstest();
}
