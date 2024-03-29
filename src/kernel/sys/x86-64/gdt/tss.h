//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Getting_to_Ring_3

#ifndef _TSS_H
#define _TSS_H

#include <types.h>

struct tss_entry_struct {
    uint32_t prev_tss;  // The previous TSS - if we used hardware task switching this would form a linked list.
    uint32_t esp0;      // The stack pointer to load when we change to kernel mode.
    uint32_t ss0;       // The stack segment to load when we change to kernel mode.
    uint32_t esp1;      // everything below here is unusued now..
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

void tss_flush();

#endif