/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_PROC_H
#define _SYS_PROC_H

#include <sys/asm/asm.h>

typedef struct proc_info {
    uint64_t id;
    pttentry cr3;
    proc_register rsp;
    proc_register rbp;
    proc_register rip;
} proc_info_t;

typedef proc_info_t********* ptable_t;

typedef uint64_t pid_t;

extern ptable_t ptable;

// proc_init.c
void proc_init();

// proc_table.c
proc_info_t* get_proc_info(pid_t pid);

#endif