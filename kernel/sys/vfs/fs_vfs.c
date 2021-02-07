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

void filesystem_vfs_open(struct vfs* v, uint8_t read, uint8_t write) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->name);
}

uint32_t filesystem_vfs_read(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->name);
    return 0;
}

uint32_t filesystem_vfs_write(struct vfs* v, uint32_t offset, uint32_t size, uint8_t* buffer) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->name);
    return 0;
}

void filesystem_vfs_close(struct vfs* v) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->name);
}

void filesystem_vfs_readdir(struct vfs* v, uint32_t index) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(v->name);
}

struct vfs* vfs_new_filesystem(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    struct vfs* ret = (struct vfs*)kmalloc(sizeof(struct vfs));
    memset((uint8_t*)ret, 0, sizeof(struct vfs));
    ret->type = filesystem;
    ret->close = &filesystem_vfs_close;
    ret->open = &filesystem_vfs_open;
    ret->read = &filesystem_vfs_read;
    ret->write = &filesystem_vfs_write;
    ret->readdir = &filesystem_vfs_readdir;
    vfs_set_name(ret, name);
    return ret;
}
