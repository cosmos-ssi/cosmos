/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/objects/objects.h>
#include <sys/panic/panic.h>
#include <sys/sched/sched.h>

void sched_switch(linkedlist* task) {
    object_handle_t proc_obj, body_obj;

    proc_obj = TASK_DATA(task)->obj;

    switch (object_type(proc_obj)) {
        case OBJECT_PROCESS:
            break;
        default:
            PANIC("Invalid object type!");
    }

    body_obj = OBJECT_DATA(proc_obj, object_process_t)->body;

    switch (object_type(body_obj)) {
        case OBJECT_KERNEL_WORK:
            OBJECT_DATA(body_obj, object_kernel_work_t)->work_func(OBJECT_DATA(body_obj, object_kernel_work_t)->arg);
            break;
        case OBJECT_EXECUTABLE:
            break;
        default:
            PANIC("Invalid object type!")
    }

    return;
}