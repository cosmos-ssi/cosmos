/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/proc/proc.h>

dtable proc_table;

void proc_table_add(proc_info_t* proc_info) {
    dtable_set(proc_table, proc_info->pid, (void*)proc_info);

    return;
}

proc_info_t* proc_table_get(pid_t pid) {
    return (proc_info_t*)dtable_get(proc_table, (uint64_t)pid);
}