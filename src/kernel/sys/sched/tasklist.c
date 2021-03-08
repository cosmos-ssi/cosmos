/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <types.h>

linkedlist* get_current_task(uint64_t cpu, uint64_t core) {
    return current_task[cpu][core];
}

linkedlist* task_find(pid_t pid) {
    uint64_t cpu, core;
    proc_info_t* p;
    linkedlist* cur;

    p = proc_table_get(pid);

    cpu = p->cpu;
    core = p->core;

    cur = task_list[cpu][core];

    while (TASK_DATA(cur)->pid != pid) {
        cur = cur->next;
    }

    return cur;
}