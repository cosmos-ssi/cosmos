/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sched/sched.h>
#include <types.h>

uint64_t syscall_sleep(uint64_t syscall_id, void* args) {
    // return 0 on failure, 1 on success
    kprintf("Sleeping...\n");

    linkedlist* task;

    task = get_current_task(CUR_CPU, CUR_CORE);

    return 0;
}