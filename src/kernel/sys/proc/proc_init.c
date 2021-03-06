/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/proc/proc.h>
#include <types.h>

uint64_t next_pid;

void proc_init() {
    proc_table = dtable_init();
    next_pid = 0;

    return;
}