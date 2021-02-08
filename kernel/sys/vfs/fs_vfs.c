//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/vfs/fs_vfs.h>

void filesystem_vfs_open(struct vfs_node* vfs) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
}

void filesystem_vfs_close(struct vfs_node* vfs) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
}

struct vfs_node* filesystem_vfs_find_node_by_id(struct vfs_node* vfs, uint32_t idx) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    /*
    * make sure the device still exists
    */
    struct device* fs_device = (struct device*)devicemgr_find_device(vfs->name);
    if (0 != fs_device) {
        ASSERT(fs_device->devicetype == FILESYSTEM);
        //    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)fs_device->api;
    }
    return 0;
}

struct vfs_node* filesystem_vfs_find_node_by_name(struct vfs_node* vfs, uint8_t* name) {
    /*
    * make sure the device still exists
    */
    struct device* fs_device = (struct device*)devicemgr_find_device(vfs->name);
    if (0 != fs_device) {
        ASSERT(fs_device->devicetype == FILESYSTEM);
        //  struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)fs_device->api;
    }
    return 0;
}

struct vfs_node* vfs_new_filesystem(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    /*
    * device exists and is of right type
    */
    struct device* fs_device = (struct device*)devicemgr_find_device(name);
    ASSERT_NOT_NULL(fs_device)
    ASSERT(fs_device->devicetype == FILESYSTEM);
    /*
    * make the node
    */
    struct vfs_node* ret = (struct vfs_node*)kmalloc(sizeof(struct vfs_node));
    memset((uint8_t*)ret, 0, sizeof(struct vfs_node));
    ret->type = folder;
    ret->close = &filesystem_vfs_close;
    ret->open = &filesystem_vfs_open;
    ret->read = 0;
    ret->write = 0;
    ret->find_id = &filesystem_vfs_find_node_by_id;
    ret->find_name = &filesystem_vfs_find_node_by_name;
    vfs_set_name(ret, name);
    return ret;
}
