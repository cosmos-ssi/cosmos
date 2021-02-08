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

uint32_t filesystem_vfs_read(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    return 0;
}

uint32_t filesystem_vfs_write(struct vfs_node* vfs, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    return 0;
}

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
    return 0;
}

struct vfs_node* filesystem_vfs_find_node_by_name(struct vfs_node* vfs, uint8_t* name) {
    ASSERT_NOT_NULL(vfs);
    ASSERT_NOT_NULL(vfs->name);
    return 0;
}

struct vfs_node* vfs_new_filesystem(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    struct vfs_node* ret = (struct vfs_node*)kmalloc(sizeof(struct vfs_node));
    memset((uint8_t*)ret, 0, sizeof(struct vfs_node));
    ret->type = folder;
    ret->close = &filesystem_vfs_close;
    ret->open = &filesystem_vfs_open;
    ret->read = &filesystem_vfs_read;
    ret->write = &filesystem_vfs_write;
    ret->find_id = &filesystem_vfs_find_node_by_id;
    ret->find_name = &filesystem_vfs_find_node_by_name;
    vfs_set_name(ret, name);
    return ret;
}
