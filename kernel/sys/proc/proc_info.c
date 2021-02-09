/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/panic/panic.h>
#include <sys/proc/proc.h>

proc_info_t* new_proc_info(pid_t pid, pttentry cr3) {
    proc_info_t* proc_info = 0;

    proc_info = (proc_info_t*)kmalloc(sizeof(proc_info_t));
    if (!proc_info) {
        panic("Unable to allocate space for proc_info struct!");
    }

    proc_info->pid = pid;
    proc_info->cr3 = cr3;

    return proc_info;
}