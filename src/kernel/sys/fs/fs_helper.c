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

struct filesystem_node* fshelper_get_fs_rootnode(struct device* filesystem_device) {
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

    uint32_t count = fshelper_count(fs_node);
    if (count > 0) {
        for (uint32_t i = 0; i < count; i++) {
            struct filesystem_node* child = fshelper_find_node_by_idx(fs_node, i);
            fshelper_traverse_internal(child, f, depth + 1);
        }
    }
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

uint32_t fshelper_count(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->count) {
        return (*fs_api->count)(fs_node);
    }
    return 0;
}

struct filesystem_node* fshelper_find_node_by_idx(struct filesystem_node* fs_node, uint32_t idx) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_device);
    ASSERT_NOT_NULL(fs_node->filesystem_device->api);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)fs_node->filesystem_device->api;
    if (0 != fs_api->find_idx) {
        return (*fs_api->find_idx)(fs_node, idx);
    }
    return 0;
}
