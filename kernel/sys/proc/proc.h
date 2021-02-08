/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_PROC_H
#define _SYS_PROC_H

#include <sys/asm/asm.h>

#define PTABLE_LEVEL_1(pid) (pid >> 56)
#define PTABLE_LEVEL_2(pid) ((pid & 0x00FF000000000000) >> 48)
#define PTABLE_LEVEL_3(pid) ((pid & 0x0000FF0000000000) >> 40)
#define PTABLE_LEVEL_4(pid) ((pid & 0x000000FF00000000) >> 32)
#define PTABLE_LEVEL_5(pid) ((pid & 0x00000000FF000000) >> 24)
#define PTABLE_LEVEL_6(pid) ((pid & 0x0000000000FF0000) >> 16)
#define PTABLE_LEVEL_7(pid) ((pid & 0x000000000000FF00) >> 8)
#define PTABLE_LEVEL_8(pid) (pid & 0x00000000000000FF)

typedef uint64_t pid_t;

typedef struct proc_info {
    pid_t pid;
    pttentry cr3;
    proc_register rsp;
    proc_register rbp;
    proc_register rip;
} proc_info_t;

typedef proc_info_t********* ptable_t;

extern ptable_t ptable;

// proc_init.c
void proc_init();

// proc_table.c
bool add_proc_entry(proc_info_t* proc);
proc_info_t* get_proc_info(pid_t pid);

#endif