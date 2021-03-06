//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct filesystem_node* fsfacade_get_fs_rootnode(struct object* filesystem_obj) {
    ASSERT_NOT_NULL(filesystem_obj);
    ASSERT_NOT_NULL(filesystem_obj->object_data);
    struct objectinterface_filesystem* fs_api = (struct objectinterface_filesystem*)filesystem_obj->api;
    ASSERT_NOT_NULL(fs_api->root);
    return (*fs_api->root)(filesystem_obj);
}

void fsfacade_dump_node(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);

    kprintf("%s (id: %llu, size: %llu, parent: %llu)\n", fs_node->name, fs_node->id, fs_node->size, fs_node->parent);

    // kprintf("FS Node Name: %s\n", fs_node->name);
    //   kprintf("FS Node ID: %#llX\n", fs_node->id);
    //   kprintf("FS Node Owner Object: %s\n", fs_node->filesystem_obj->name);
    //   kprintf("FS Node Type: %#llX\n", fs_node->type);
    //   kprintf("FS Node Data: %#llX\n", fs_node->node_data);
}

void fsfacade_traverse_internal(struct filesystem_node* fs_node, fsfacade_traverse_function f, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);

    ASSERT_NOT_NULL(f);

    // call the callback
    (*f)(fs_node, depth);

    struct filesystem_directory dir;
    memzero((uint8_t*)&dir, sizeof(struct filesystem_directory));
    fsfacade_list_directory(fs_node, &dir);

    if (dir.count > 0) {
        for (uint32_t i = 0; i < dir.count; i++) {
            //    kprintf("dir %s index: %llu child id: %#llX\n", fs_node->name, i, dir.ids[i]);
            struct filesystem_node* child = fsfacade_find_node_by_id(fs_node, dir.ids[i]);
            ASSERT_NOT_NULL(child);
            fsfacade_traverse_internal(child, f, depth + 1);
        }
    }
}

/*
 * traverse
 */
void fsfacade_traverse(struct filesystem_node* fs_node, fsfacade_traverse_function f) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    ASSERT_NOT_NULL(f);
    fsfacade_traverse_internal(fs_node, f, 0);
}

void fsfacade_voh_dump_traverse_function(struct filesystem_node* fs_node, uint32_t depth) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);

    for (int32_t i = 0; i < depth; i++) {
        kprintf("  ");
    }
    fsfacade_dump_node(fs_node);
}

void fsfacade_dump(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    fsfacade_traverse(fs_node, &fsfacade_voh_dump_traverse_function);
}

void fsfacade_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    ASSERT_NOT_NULL(dir);
    struct objectinterface_filesystem* fs_api = (struct objectinterface_filesystem*)fs_node->filesystem_obj->api;
    if (0 != fs_api->list) {
        (*fs_api->list)(fs_node, dir);
    }
}

struct filesystem_node* fsfacade_find_node_by_id(struct filesystem_node* fs_node, uint64_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    struct objectinterface_filesystem* fs_api = (struct objectinterface_filesystem*)fs_node->filesystem_obj->api;
    if (0 != fs_api->find_id) {
        return (*fs_api->find_id)(fs_node, id);
    }
    return 0;
}

uint64_t fsfacade_size(struct filesystem_node* fs_node) {
    return fs_node->size;
}

struct filesystem_node* fsfacade_find_node_by_name(struct filesystem_node* fs_node, char* name) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    ASSERT_NOT_NULL(name);

    struct filesystem_directory dir;
    fsfacade_list_directory(fs_node, &dir);

    if (dir.count > 0) {
        for (uint32_t i = 0; i < dir.count; i++) {
            uint64_t id = dir.ids[i];
            struct filesystem_node* node = fsfacade_find_node_by_id(fs_node, id);
            ASSERT_NOT_NULL(node);
            if (strcmp(node->name, name) == 0) {
                return node;
            }
        }
    }
    return 0;
}

uint32_t fsfacade_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    struct objectinterface_filesystem* fs_api = (struct objectinterface_filesystem*)fs_node->filesystem_obj->api;
    if (0 != fs_api->read) {
        return (*fs_api->read)(fs_node, data, data_size);
    }
    return 0;
}

uint32_t fsfacade_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->api);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    struct objectinterface_filesystem* fs_api = (struct objectinterface_filesystem*)fs_node->filesystem_obj->api;
    if (0 != fs_api->write) {
        return (*fs_api->write)(fs_node, data, data_size);
    }
    return 0;
}

void dump_VOH() {
    //    kprintf("\n");
    //    kprintf("***** VOH (Virtual Object Hierarchy) *****\n");
    kprintf("\n");
    struct object* voh_dev = objectmgr_find_object_by_name("voh0");
    ASSERT_NOT_NULL(voh_dev);
    struct filesystem_node* fs_node = fsfacade_get_fs_rootnode(voh_dev);
    ASSERT_NOT_NULL(fs_node);

    fsfacade_dump(fs_node);
}
