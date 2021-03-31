//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_console.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/sched/sched.h>
#include <sys/syscall/syscalls_process.h>

uint64_t syscall_process_exit(uint64_t syscall_id, struct syscall_args* args) {
    // exit
    linkedlist* task;

    uint64_t exit_code;

    exit_code = args->arg1;

    task = get_current_task(CUR_CPU, CUR_CORE);

    TASK_DATA(task)->exit_code = exit_code;

    sched_terminate(TASK_DATA(task)->pid);
    sched_switch(task_select());

    return 0;
}

uint64_t syscall_process_sleep(uint64_t syscall_id, struct syscall_args* args) {
    kprintf("syscall %llu\n not implemented", syscall_id);
    return 0;
}
