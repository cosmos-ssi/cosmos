/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/panic/panic.h>
#include <sys/sched/sched.h>

object_handle_t object_create_process(object_handle_t exe, pid_t pid) {
    object_process_t* obj;

    obj = (object_process_t*)kmalloc(sizeof(object_process_t));
    if (!obj) {
        PANIC("kmalloc failed!");
    }

    obj->body = exe;
    obj->pid = pid;

    return object_create(OBJECT_PROCESS, (void*)obj);
}

object_handle_t object_create_task(object_handle_t proc) {
    linkedlist* sched_task;
    pid_t pid;

    pid = ((object_process_t*)((object_t*)dtable_get(object_table, proc))->data)->pid;

    sched_task = sched_add(0, 0, pid);

    return 0;
}