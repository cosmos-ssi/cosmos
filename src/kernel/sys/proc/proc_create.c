/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/proc/proc.h>
#include <sys/string/mem.h>
#include <sys/sync/sync.h>

pid_t proc_create() {
    proc_info_t* proc_info;

    proc_info = (proc_info_t*)kmalloc(sizeof(proc_info_t));
    ASSERT_NOT_NULL(proc_info);
    memset((uint8_t*)proc_info, 0, sizeof(proc_info_t));

    spinlock_acquire(&proc_table_lock);
    proc_info->pid = get_next_pid();

    spinlock_release(&proc_table_lock);
    return proc_info->pid;
}