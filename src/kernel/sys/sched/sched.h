/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SCHED_H
#define _SCHED_H

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <types.h>

// just to simplify a soup of parentheses
#define TASK_LIST_DATA(x, y) ((scheduler_task_t*)task_list[x][y]->data)
#define CURRENT_TASK_DATA(x, y) ((scheduler_task_t*)current_task[x][y]->data)
#define TASK_DATA(x) ((scheduler_task_t*)(x->data))
#define TASK_LIST_ADJUST(x, y) (task_list[x][y] = task_list[x][y]->next)

// Replace these with something more sophisticated when appropriate
#define CUR_CPU 0
#define CUR_CORE 0

typedef enum scheduler_state_t {
    SCHED_RUNNING,    // duh
    SCHED_SLEEPING,   // currently awaiting rescheduling
    SCHED_IOWAIT,     // awaiting IO, do not schedule until received
    SCHED_ZOMBIE,     // In your he-ead, in your he-e-e-ead...
    SCHED_TERMINATE,  // Terminated, waiting for kernel tasklet to clean up
    SCHED_LASTRESORT  // only schedule if no other schedulable tasks
} scheduler_state_t;

typedef struct scheduler_task_t {
    pid_t pid;
    scheduler_state_t state;
    uint64_t exit_code;

    /*
     * Processes that have registered to be notified of this task's termination
     * and exit code.
     */
    linkedlist* notify_term;

    /* If, when going through the task list deciding which task to do next, we
     * decide not to switch to this one for any reason, increment times_skipped.
     * Reset to zero whenever this task IS the one switched to.
     */
    uint64_t times_skipped;

    object_handle_t obj;
} scheduler_task_t;

// one each for each processor/core combo
extern linkedlist*** current_task;

/*
 * Note that functions in collection/linkedlist generally won't work on this
 * because they assume a definite end, while this is going to be
 * circularly-linked
 */
extern linkedlist*** task_list;

// sched.c
void sched_switch();

// sched_add.c
linkedlist* sched_add(uint64_t cpu, uint64_t core, pid_t pid, object_handle_t obj);

// sched_init.c
void sched_init();

// sched_terminate.c
void sched_terminate();

// tasklist.c
linkedlist* task_find(pid_t pid);

// task_select.c
linkedlist* task_select();

#endif