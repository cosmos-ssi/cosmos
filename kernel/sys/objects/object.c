/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>
#include <sys/panic/panic.h>

object_handle_t object_create(object_types_t type, void* object_data) {
    object_t* obj;

    obj = (object_t*)kmalloc(sizeof(object_t));

    if (!obj) {
        panic("kmalloc failed!");
    }

    obj->data = object_data;

    dtable_set(object_table, object_table_next_idx, obj);

    // return value BEFORE increment
    return object_table_next_idx++;
}