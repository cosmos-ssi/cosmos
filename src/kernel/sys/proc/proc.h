/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_PROC_H
#define _SYS_PROC_H

#include <sys/collection/dtable/dtable.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

typedef uint8_t fpu_reg[10];

typedef uint64_t xmm_reg[2];

typedef uint64_t pid_t;

typedef struct proc_info {
    pid_t pid;
    pttentry cr3;
    uint64_t cpu;
    uint64_t core;
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

extern dtable proc_table;
extern uint64_t next_pid;

// kernel_idle.c
void* kernel_idle(void* arg);

// pid.c
pid_t get_next_pid();

// proc_create.c
pid_t proc_create();
void setup_user_process(pid_t pid);

// proc_init.c
void proc_init();

// proc_table.c
void proc_table_add(proc_info_t* proc_info);

#endif