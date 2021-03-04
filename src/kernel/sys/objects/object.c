/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/collection/dtable/dtable.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objects/objects.h>

object_handle_t object_create(object_types_t type, void* object_data) {
    object_t* obj;

    obj = (object_t*)kmalloc(sizeof(object_t));
    obj->type = type;
    obj->name = 0;

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

    // object knows its own handle; we want the value BEFORE increment
    obj->handle = object_table_next_idx++;

    return obj->handle;
}

object_types_t object_type_(object_handle_t obj) {
    object_t* o;

    o = object_table_get(obj);

    return o->type;
}