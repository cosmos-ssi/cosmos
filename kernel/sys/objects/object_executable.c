/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/logical/fs/initrd/initrd.h>
#include <sys/devicemgr/device.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objects/objects.h>
#include <sys/string/string.h>
#include <sys/x86-64/mm/mm.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

object_handle_t object_create_executable_from_presentation(object_handle_t pres_handle) {
    object_presentation_t* pres_obj;
    object_executable_t* exe_obj;
    uint64_t name_len;
    uint32_t pres_len;

    device_t* initrd;

    pres_obj = (object_presentation_t*)object_get_data(pres_handle);

    exe_obj = (object_executable_t*)kmalloc(sizeof(obj_executable_t));

    name_len = strlen(pres_obj->vfs_name);
    exe_obj->exe_name = (char*)kmalloc(sizeof(char) * (name_len + 1));
    strncpy((uint8_t*)exe_obj->exe_name, (uint8_t*)pres_obj->vfs_name, name_len + 1);

    initrd = initrd_attach(pres_obj->dev, initrd_lba());

    pres_len = initrd_get_file_length(initrd, pres_obj->idx);

    exe_obj->page_count = (pres_len / PAGE_SIZE) + ((pres_len % PAGE_SIZE) ? 1 : 0);

    exe_obj->page_base = slab_allocate(exe_obj->page_count, PDT_INUSE);

    exe_obj->from_presentation = true;
    exe_obj->presentation = pres_handle;

    return 0;
}