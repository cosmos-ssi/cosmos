/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/proc/proc.h>

void proc_init() {
    ptable = 0;
    proc_info_t* kernelproc;

    kernelproc = new_proc_info(0, asm_cr3_read());
    if (!add_proc_entry(kernelproc)) {
        PANIC("Unable to add kernel process to table!");
    }

    return;
}