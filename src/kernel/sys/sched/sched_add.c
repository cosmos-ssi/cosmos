/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <types.h>

void sched_add(uint64_t cpu, uint64_t core, pid_t pid) {
    scheduler_task_t* new_task;
    linkedlist* new_list_entry;

    new_task = (scheduler_task_t*)kmalloc(sizeof(scheduler_task_t));

    new_task->pid = pid;

    /*
     * All new tasks begin in the SCHED_SLEEPING state; they do not begin
     * execution until the scheduler schedules them.
     */
    new_task->state = SCHED_SLEEPING;

    new_task->exit_code = 0;
    new_task->times_skipped = 0;

    new_list_entry = linkedlist_new();

    new_list_entry->data = (void*)new_task;

    new_list_entry->next = task_list[cpu][core]->next;
    task_list[cpu][core]->next = new_list_entry;

    return;
}