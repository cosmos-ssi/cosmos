/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/sched/sched.h>

uint64_t** current_task;

void sched_init() {
    /*
     * Initialize current_task array.  Right now we only support a single
     * processor, so this is simpler than it will become in the future.
     */

    current_task = (uint64_t**)kmalloc(sizeof(uint64_t));
    current_task[0] = (uint64_t*)kmalloc(sizeof(uint64_t));
    current_task[0][0] = 0;
}