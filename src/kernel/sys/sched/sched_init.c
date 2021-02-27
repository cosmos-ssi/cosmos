/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/sched/sched.h>

#include <sys/kprintf/kprintf.h>

linkedlist*** task_list;
linkedlist*** current_task;

void sched_init() {
    /*
     * Initialize task_list array.  Right now we only support a single
     * processor, so this is simpler than it will become in the future.
     */

    task_list = (linkedlist***)kmalloc(sizeof(linkedlist**));

    task_list[0] = (linkedlist**)kmalloc(sizeof(linkedlist*));
    task_list[0][0] = 0;

    current_task = (linkedlist***)kmalloc(sizeof(linkedlist**));
    current_task[0] = (linkedlist**)kmalloc(sizeof(linkedlist*));

    current_task[0][0] = 0;

    return;
}