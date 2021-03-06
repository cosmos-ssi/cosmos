/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>
#include <sys/sync/sync.h>
#include <types.h>

linkedlist* sched_add(uint64_t cpu, uint64_t core, pid_t pid, object_handle_t obj) {
    scheduler_task_t* new_task;
    linkedlist* new_list_entry;

    new_task = (scheduler_task_t*)kmalloc(sizeof(scheduler_task_t));

    new_task->pid = pid;
    new_task->notify_term = 0;

    /*
     * All new tasks begin in the SCHED_SLEEPING state; they do not begin
     * execution until the scheduler schedules them.
     */
    new_task->state = SCHED_SLEEPING;

    new_task->exit_code = 0;
    new_task->times_skipped = 0;
    new_task->obj = obj;

    new_list_entry = linkedlist_new();

    new_list_entry->data = (void*)new_task;

    spinlock_acquire(&task_list_lock);

    if (task_list[cpu][core]) {
        new_list_entry->next = task_list[cpu][core]->next;
        task_list[cpu][core]->next = new_list_entry;
    } else {
        new_list_entry->next = new_list_entry;
    }
    task_list[cpu][core] = new_list_entry;

    if (!current_task[cpu][core]) {
        current_task[cpu][core] = new_list_entry;
    }

    spinlock_release(&task_list_lock);

    return new_list_entry;
}