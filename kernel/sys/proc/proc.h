/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_PROC_H
#define _SYS_PROC_H

#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

#define PTABLE_LEVEL_1(pid) (pid >> 56)
#define PTABLE_LEVEL_2(pid) ((pid & 0x00FF000000000000) >> 48)
#define PTABLE_LEVEL_3(pid) ((pid & 0x0000FF0000000000) >> 40)
#define PTABLE_LEVEL_4(pid) ((pid & 0x000000FF00000000) >> 32)
#define PTABLE_LEVEL_5(pid) ((pid & 0x00000000FF000000) >> 24)
#define PTABLE_LEVEL_6(pid) ((pid & 0x0000000000FF0000) >> 16)
#define PTABLE_LEVEL_7(pid) ((pid & 0x000000000000FF00) >> 8)
#define PTABLE_LEVEL_8(pid) (pid & 0x00000000000000FF)

typedef uint8_t fpu_reg[10];

typedef uint64_t xmm_reg[2];

typedef uint64_t pid_t;

typedef struct proc_info {
    pid_t pid;
    pttentry cr3;
    proc_register rax;
    proc_register rbx;
    proc_register rcx;
    proc_register rdx;
    proc_register rsi;
    proc_register rdi;
    proc_register rsp;
    proc_register rbp;
    proc_register r8;
    proc_register r9;
    proc_register r10;
    proc_register r11;
    proc_register r12;
    proc_register r13;
    proc_register r14;
    proc_register r15;
    proc_register rip;
    proc_register rflags;
    fpu_reg st0;
    fpu_reg st1;
    fpu_reg st2;
    fpu_reg st3;
    fpu_reg st4;
    fpu_reg st5;
    fpu_reg st6;
    fpu_reg st7;
    xmm_reg xmm0;
    xmm_reg xmm1;
    xmm_reg xmm2;
    xmm_reg xmm3;
    xmm_reg xmm4;
    xmm_reg xmm5;
    xmm_reg xmm6;
    xmm_reg xmm7;
    xmm_reg xmm8;
    xmm_reg xmm9;
    xmm_reg xmm10;
    xmm_reg xmm11;
    xmm_reg xmm12;
    xmm_reg xmm13;
    xmm_reg xmm14;
    xmm_reg xmm15;
} proc_info_t;

typedef proc_info_t********* ptable_t;

extern ptable_t ptable;

// proc_init.c
proc_info_t* new_proc_info(pid_t pid, pttentry cr3);
void proc_init();

// proc_table.c
bool add_proc_entry(proc_info_t* proc);
proc_info_t* get_proc_info(pid_t pid);

#endif