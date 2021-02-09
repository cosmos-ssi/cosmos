//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/vfs/vfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/device.h>
#include <sys/fs/fs_helper.h>

struct filesystem_node* fshelper_get_fs_node(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)filesystem_device->api;
    ASSERT_NOT_NULL(fs_api->root);
    return (*fs_api->root)(filesystem_device);
}

void fshelper_traverse_internal(struct filesystem_node* fs_node, fshelper_traverse_function f, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(f);

    // call the callback
    (*f)(fs_node, depth);

    // children
    // if (0 != v->children) {
    //     for (uint32_t i = 0; i < arraylist_count(v->children); i++) {
    //         struct vfs_node* c = (struct vfs_node*)arraylist_get(v->children, i);
    //         vfs_traverse_internal(c, f, depth + 1);
    //     }
    // }
}

/*
 * traverse
 */
void fshelper_traverse(struct filesystem_node* fs_node, fshelper_traverse_function f) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(f);
    fshelper_traverse_internal(fs_node, f, 0);
}

void vfs_dump_traverse_function(struct filesystem_node* fs_node, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    for (int32_t i = 0; i < depth; i++) {
        kprintf(" ");
    }
    kprintf("%s %llu\n", fs_node->name, fs_node->id);
}

void fshelper_dump(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    fshelper_traverse(fs_node, &vfs_dump_traverse_function);
}
