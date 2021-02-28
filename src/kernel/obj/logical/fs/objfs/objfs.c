//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/node_cache.h>
#include <obj/logical/fs/node_util.h>
#include <obj/logical/fs/objfs/objfs.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <types.h>

struct objfs_objectdata {
    struct filesystem_node* root_node;
    struct node_cache* nc;
};

/*
 * perform device instance specific init here
 */
uint8_t objfs_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

uint64_t objfs_node_id(uint64_t device_type, uint64_t device_number) {
    //   kprintf("device_type %#lllX device number %#llX\n", device_type, device_number);
    return (device_type << 32) + device_number;
}

uint64_t objfs_device_type(uint64_t node_id) {
    return node_id >> 32;
}

uint64_t objfs_device_number(uint64_t node_id) {
    return (node_id & 0xFFFF0000);
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t objfs_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s  (%s)\n", obj->description, obj->name);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)obj->object_data;
    kfree(obj->api);
    kfree(object_data->root_node);
    node_cache_delete(object_data->nc);
    kfree(object_data);
    return 1;
}

struct filesystem_node* objfs_get_root_node(struct object* filesystem_obj) {
    ASSERT_NOT_NULL(filesystem_obj);
    ASSERT_NOT_NULL(filesystem_obj->object_data);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)filesystem_obj->object_data;
    return object_data->root_node;
}

uint32_t objfs_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // read from node. we cant read from the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");

    return 0;
}

uint32_t objfs_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // write to node. we cant write to the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");

    return 0;
}

void objfs_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

void objfs_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

struct filesystem_node* objfs_find_node_by_id(struct filesystem_node* fs_node, uint64_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)fs_node->filesystem_obj->object_data;
    //  kprintf("finding %#llX in %s\n", id, fs_node->name);

    /*
    * check the cache
    */
    struct filesystem_node* this_node = node_cache_find(object_data->nc, id);
    if (0 == this_node) {
        enum object_type_id dt = (enum object_type_id)objfs_device_type(id);
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            // there is a node with that id, we need to make a fs entry and cache it
            this_node =
                filesystem_node_new(folder, fs_node->filesystem_obj, object_type_names[id], objfs_node_id(id, 0), 0);
            node_cache_add(object_data->nc, this_node);
        }
    } else {
        //    kprintf("found in cache %llu\n", id);
    }
    if (0 == this_node) {
        kprintf("no node with id %#llX\n", id);
    }
    return this_node;
}

void objfs_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    kprintf("objfs_list_directory %s\n", fs_node->name);
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT_NOT_NULL(dir);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * root node
        */
        dir->count = objecttypes_count();
        uint32_t folder_count = 0;
        /*
        * every device type has a unique integer to identify it, so that can be the node_id
        */
        for (uint32_t i = 0; i < MAX_OBJECT_TYPES; i++) {
            struct arraylist* lst = objecttypes_get_objectlist(i);
            if (0 != lst) {
                struct filesystem_node* this_node = node_cache_find(object_data->nc, i);
                if (0 == this_node) {
                    //             kprintf("node_id %#llX %#llX\n", i, objfs_node_id(i, 0));
                    this_node = filesystem_node_new(folder, fs_node->filesystem_obj, object_type_names[i],
                                                    objfs_node_id(i, 0), 0);
                    node_cache_add(object_data->nc, this_node);
                }
                dir->ids[folder_count] = this_node->id;
                folder_count += 1;
            }
        }
    } else {
        /*
        * folders contain leaf nodes for the type
        */
        if (fs_node->type == folder) {

            kprintf("folder %s %#llX\n", fs_node->name, fs_node->id);

            enum object_type_id dt = (enum object_type_id)objfs_device_type(fs_node->id);
            ASSERT_NOT_NULL(dt);
            //   kprintf("dt %#llX\n", dt);
            struct arraylist* lst = objecttypes_get_objectlist(dt);
            ASSERT_NOT_NULL(lst);
            uint32_t count = arraylist_count(lst);
            dir->count = count;

            for (uint32_t i = 0; i < count; i++) {
                uint64_t node_id = objfs_node_id(dt, i + 1);
                struct filesystem_node* this_node = node_cache_find(object_data->nc, node_id);
                if (0 == this_node) {
                    struct object* obj = (struct object*)arraylist_get(lst, i);
                    ASSERT_NOT_NULL(obj);
                    kprintf("dev %s\n", obj->name);

                    this_node = filesystem_node_new(object, obj, obj->name, node_id, 0);
                    kprintf("dev2 %s\n", obj->name);

                    node_cache_add(object_data->nc, this_node);
                    kprintf("dev3 %s\n", obj->name);
                }
                dir->ids[i] = this_node->id;
            }
        } else {
            // its a leaf node which is a device. it has no children
        }
    }
}

uint64_t objfs_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    // devices have no size
    return 0;
}

struct object* objfs_attach() {
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &objfs_init;
    objectinstance->uninit = &objfs_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = DEVFS;
    objectinstance->object_data = 0;
    objectmgr_set_object_description(objectinstance, "Object File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    api->close = &objfs_close;
    api->find_id = &objfs_find_node_by_id;
    api->open = &objfs_open;
    api->root = &objfs_get_root_node;
    api->write = &objfs_write;
    api->read = &objfs_read;
    api->list = &objfs_list_directory;
    api->size = &objfs_size;
    objectinstance->api = api;
    /*
     * device data
     */
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)kmalloc(sizeof(struct objfs_objectdata));
    object_data->root_node = filesystem_node_new(folder, objectinstance, "dev", 0, 0);
    object_data->nc = node_cache_new();
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * return device
        */
        return objectinstance;
    } else {
        node_cache_delete(object_data->nc);
        kfree(object_data->root_node);
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void objfs_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
