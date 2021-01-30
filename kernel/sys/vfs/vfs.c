//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/string.h>
#include <sys/vfs/folder_vfs.h>
#include <sys/vfs/vfs.h>

struct vfs* cosmos_vfs = 0;

void vfs_open(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void vfs_read(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void vfs_write(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}
void vfs_close(struct vfs* v) {
    ASSERT_NOT_NULL(v);
}

void vfs_init() {
    cosmos_vfs = vfs_new_folder("/");
    struct vfs* dev_folder = vfs_new_folder("dev");
    vfs_add_child(cosmos_vfs, dev_folder);
}

void vfs_delete(struct vfs* v) {
    ASSERT_NOT_NULL(v);
    if (0 != v->children) {
        uint32_t count = arraylist_count(v->children);
        for (uint32_t i = 0; i < count; i++) {
            struct vfs* child = (struct vfs*)arraylist_get(v->children, i);
            vfs_delete(child);
        }
    }
    kfree(v->name);
}

void vfs_set_name(struct vfs* v, uint8_t* name) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(name);

    if (0 != v->name) {
        kfree(v->name);
    }
    uint32_t size = strlen(name) + 1;
    v->name = kmalloc(size);
    strncpy(v->name, name, size);
}

void vfs_add_child(struct vfs* v, struct vfs* child) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(child);
    ASSERT_NOT_NULL(child->name);
    if (0 == v->children) {
        v->children = arraylist_new();
    }
    arraylist_add(v->children, child);
}

struct vfs* vfs_find(struct vfs* v, uint8_t* name) {
    ASSERT_NOT_NULL(v);
    ASSERT_NOT_NULL(name);
    panic("Not Implemented");

    if (0 == strcmp(name, v->name)) {
        // its this node
        return v;
    } else {
        if (0 != v->children) {
            // recurse
            uint32_t idx = strstr(name, "/");
            if (-1 != idx) {
                //     uint8_t* n = &(name[idx]);
            }
            return 0;
        } else {
            // no luck
            return 0;
        }
    }
}