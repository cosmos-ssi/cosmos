/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>

typedef struct proc_info {
    uint64_t id;
    pttentry cr3;
    proc_register rsp;
    proc_register rbp;
    proc_register rip;
} proc_info;

typedef proc_info******** ptable;

ptable proc_table;