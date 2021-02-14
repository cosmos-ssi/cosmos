/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/collection/dtable/dtable.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/panic/panic.h>
#include <sys/proc/proc.h>

uint64_t next_pid;

void proc_init() {
    proc_info_t* kernelproc;

    proc_table = dtable_init();

    kernelproc = new_proc_info(0, asm_cr3_read());
    if (!dtable_set(proc_table, 0, (void*)kernelproc)) {
        PANIC("Unable to add kernel process to table!");
    }

    next_pid = 1;

    return;
}