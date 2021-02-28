/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/logical/fs/initrd/initrd.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/objects/objects.h>
#include <sys/panic/panic.h>

object_handle_t object_presentation_create(filesystem_node_t* node) {
    object_presentation_t* obj_data;

    obj_data = (object_presentation_t*)kmalloc(sizeof(object_presentation_t));
    if (!obj_data) {
        PANIC("kmalloc failed!");
    }

    /* 
     * No attempt to validate any of this for now, once vfs is stabilized that
     * will be added
     */

    obj_data->node = node;

    return object_create(OBJECT_PRESENTATION, (void*)obj_data);
}