/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objectinterface/objectinterface_filesystem.h>
#include <sys/objects/objects.h>
#include <sys/string/string.h>
#include <sys/x86-64/mm/mm.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

object_handle_t object_executable_create_from_presentation(object_handle_t pres_handle) {
    object_presentation_t* pres_obj;
    object_executable_t* exe_obj;
    uint64_t name_len;
    uint32_t pres_len;
    object_handle_t exe_handle;
    BYTE* exe_buf;
    filesystem_node_t* node;

    pres_obj = OBJECT_DATA(pres_handle, object_presentation_t);
    node = pres_obj->node;

    exe_obj = (object_executable_t*)kmalloc(sizeof(obj_executable_t));

    name_len = strlen(node->name);
    exe_obj->exe_name = (char*)kmalloc(sizeof(char) * (name_len + 1));
    strncpy((uint8_t*)exe_obj->exe_name, (uint8_t*)node->name, name_len + 1);

    pres_len = fsfacade_size(node);
    // not all devices that implement deviceapi_filesystem may implement the "size" api
    ASSERT_NOT_NULL(pres_len);

    exe_obj->page_count = (pres_len / PAGE_SIZE) + ((pres_len % PAGE_SIZE) ? 1 : 0);

    exe_obj->page_base = slab_allocate(exe_obj->page_count, PDT_INUSE);

    exe_buf = (BYTE*)CONV_PHYS_ADDR((exe_obj->page_base * PAGE_SIZE));
    fsfacade_read(node, (uint8_t*)exe_buf, PAGE_SIZE * exe_obj->page_count);

    exe_obj->from_presentation = true;
    exe_obj->presentation = pres_handle;

    exe_handle = object_create(OBJECT_EXECUTABLE, (void*)exe_obj);

    return exe_handle;
}