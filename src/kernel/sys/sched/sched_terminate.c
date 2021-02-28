/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/proc/proc.h>
#include <sys/sched/sched.h>

void sched_terminate(pid_t pid) {
    linkedlist* task;

    task = task_find(pid);

    TASK_DATA(task)->state = SCHED_TERMINATE;

    return;
}