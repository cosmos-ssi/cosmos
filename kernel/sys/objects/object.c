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
        return 0;
    }

    obj->data = object_data;

    /*
     * To allow functions that return an object_handle_t to return 0 on error,
     * the minimum handle # is 1. However, we still want zero-based numbering
     * for object_table, so we subtract 1 when placing it in the table.  We also
     * will have to do a similar adjustment when getting object data.
     */
    dtable_set(object_table, object_table_next_idx - 1, obj);

    // return value BEFORE increment
    return object_table_next_idx++;
}