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
#include <types.h>

object_handle_t object_task_create(object_handle_t proc) {
    linkedlist* sched_task;
    pid_t pid;
    object_task_t* obj;

    obj = (object_task_t*)kmalloc(sizeof(object_task_t));

    obj->process = proc;

    pid = (OBJECT_DATA(proc, object_process_t))->pid;

    sched_task = sched_add(CUR_CPU, CUR_CORE, pid, proc);
    obj->sched_task = sched_task;

    return object_create(OBJECT_TASK, (void*)obj);
}