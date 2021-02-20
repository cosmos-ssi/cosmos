/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SCHED_H
#define _SCHED_H

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/proc/proc.h>
#include <types.h>

#define SCHEDULER_TASK_LIST(x, y) ((scheduler_task_t*)task_list[x][y]->data)
#define SCHEDULER_TASK_LIST_ADJUST(x, y) (task_list[x][y] = task_list[x][y]->next)

typedef enum scheduler_state_t {
    SCHED_RUNNING,
    SCHED_SLEEPING,
    SCHED_IOWAIT,
    SCHED_ZOMBIE  // In your he-ead, in your he-e-e-ead...
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
} scheduler_task_t;

// one each for each processor/core combo
extern uint64_t** current_task;

/*
 * Note that functions in collection/linkedlist generally won't work on this
 * because they assume a definite end, while this is going to be
 * circularly-linked
 */
extern linkedlist*** task_list;

// sched_add.c
void sched_add(uint64_t cpu, uint64_t core, pid_t pid);

// sched_init.c
void sched_init();

// sched_terminate.c
void sched_terminate();

#endif