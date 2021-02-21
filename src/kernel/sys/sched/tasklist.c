/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/proc/proc.h>
#include <sys/sched/sched.h>

linkedlist* task_find(pid_t pid) {
    uint64_t cpu, core;
    proc_info_t* p;
    linkedlist* cur;

    p = dtable_get(proc_table, pid);

    cpu = p->cpu;
    core = p->core;

    cur = task_list[cpu][core];

    while (TASK_DATA(cur)->pid != p) {
        cur = cur->next;
    }

    return cur;
}