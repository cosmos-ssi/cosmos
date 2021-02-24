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

object_handle_t object_process_create(object_handle_t exe) {
    object_process_t* obj;

    obj = (object_process_t*)kmalloc(sizeof(object_process_t));
    ASSERT_NOT_NULL(obj);

    obj->body = exe;
    obj->pid = proc_create();

    return object_create(OBJECT_PROCESS, (void*)obj);
}