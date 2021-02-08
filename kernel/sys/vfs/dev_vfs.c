//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/vfs/dev_vfs.h>

uint32_t dev_vfs_read(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    return 0;
}

uint32_t dev_vfs_write(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    return 0;
}

void dev_vfs_open(struct vfs_node* vfs) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
}

void dev_vfs_close(struct vfs_node* vfs) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
}

struct vfs_node* vfs_new_dev(uint8_t* devicename) {
    ASSERT_NOT_NULL(devicename);
    struct vfs_node* ret = (struct vfs_node*)kmalloc(sizeof(struct vfs_node));
    memset((uint8_t*)ret, 0, sizeof(struct vfs_node));
    ret->children = 0;
    ret->type = device;
    ret->close = &dev_vfs_close;
    ret->open = &dev_vfs_open;
    ret->read = &dev_vfs_read;
    ret->write = &dev_vfs_write;
    ret->find_id = 0;    // devices are terminal nodes
    ret->find_name = 0;  // devices are terminal nodes

    vfs_set_name(ret, devicename);
    return ret;
}
