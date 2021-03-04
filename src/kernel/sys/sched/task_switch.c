/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <types.h>

void switch_to_task(linkedlist* task) {
    uint64_t proc_obj;
    pid_t pid;
    proc_info_t* proc;

    pid = TASK_DATA(task)->pid;
    proc_obj = TASK_DATA(task)->obj;

    proc = proc_table_get(pid);

    task_jump(proc);

    return;
}