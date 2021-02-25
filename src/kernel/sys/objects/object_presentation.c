/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>
#include <sys/panic/panic.h>
#include <sys/string/string.h>
#include <types.h>

object_handle_t object_presentation_create(device_t* dev, uint8_t idx, const char* name) {
    object_presentation_t* obj_data;
    object_handle_t handle;

    obj_data = (object_presentation_t*)kmalloc(sizeof(object_presentation_t));
    if (!obj_data) {
        PANIC("kmalloc failed!");
    }

    /* 
     * No attempt to validate any of this for now, once vfs is stabilized that
     * will be added
     */

    obj_data->dev = dev;
    obj_data->idx = idx;
    uint64_t len = sizeof(char) * (strlen(name) + 1);
    obj_data->vfs_name = (char*)kmalloc(len);
    strncpy(obj_data->vfs_name, name, len);

    handle = object_create(OBJECT_PRESENTATION, (void*)obj_data);

    return handle;
}