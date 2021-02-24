/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/objects/objects.h>
#include <sys/sched/sched.h>

object_handle_t object_task_create(object_handle_t proc) {
    linkedlist* sched_task;
    pid_t pid;

    pid = ((object_process_t*)OBJECT_DATA(proc))->pid;

    sched_task = sched_add(0, 0, pid);

    return 0;
}