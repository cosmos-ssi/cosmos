//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/fs/file_util.h>
#include <sys/fs/fs_facade.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>

/*
* read a file. this kmallocs new memory and returns it.  The file len is populated into len
*/
uint8_t* file_util_read_file(uint8_t* device_name, uint8_t* file_name, uint32_t* len) {
    ASSERT_NOT_NULL(device_name);
    ASSERT_NOT_NULL(file_name);
    ASSERT_NOT_NULL(len);
    /*
    * get device
    */
    struct object* fs_dev = objectmgr_find_object(device_name);
    if (0 != fs_dev) {
        /*
        * root node
        */
        struct filesystem_node* fs_root_node = fsfacade_get_fs_rootnode(fs_dev);
        /*
        * file node
        */
        struct filesystem_node* fs_node = fsfacade_find_node_by_name(fs_root_node, file_name);
        if (0 != fs_node) {
            uint32_t buffer_size = fsfacade_size(fs_node);
            *len = buffer_size;
            uint8_t* ret = kmalloc(buffer_size);
            fsfacade_read(fs_node, ret, buffer_size);
            return ret;
        } else {
            kprintf("Unable to find file %s for Font\n", file_name);
        }
    } else {
        kprintf("Unable to find fs device %s for Font\n", device_name);
    }
    return 0;
}