/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SCHED_H
#define _SCHED_H

#include <sys/proc/proc.h>
#include <types.h>

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

    /* If, when going through the task list deciding which task to do next, we
     * decide not to switch to this one for any reason, increment times_skipped.
     * Reset to zero whenever this task IS the one switched to.
     */
    uint64_t times_skipped;
} scheduler_task_t;

// one each for each processor/core combo
extern uint64_t** current_task;

// sched_add.c
void sched_add();

// sched_init.c
void sched_init();

#endif