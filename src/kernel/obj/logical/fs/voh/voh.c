//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/node_util.h>
#include <obj/logical/fs/voh/voh.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>

struct voh_objectdata {
    struct arraylist* children;
    struct filesystem_node* root_node;
};

/*
 * perform device instance specific init here
 */
uint8_t voh_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t voh_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s  (%s)\n", obj->description, obj->name);
    struct voh_objectdata* object_data = (struct voh_objectdata*)obj->object_data;

    kfree(obj->api);
    kfree(object_data->root_node);
    kfree(object_data->children);
    kfree(object_data);
    return 1;
}

struct filesystem_node* voh_get_root_node(struct object* filesystem_obj) {
    ASSERT_NOT_NULL(filesystem_obj);
    ASSERT_NOT_NULL(filesystem_obj->object_data);
    struct voh_objectdata* object_data = (struct voh_objectdata*)filesystem_obj->object_data;
    return object_data->root_node;
}

uint32_t voh_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // read from node. we cant read from the root node, but we can find underlying file and folder nodes
    PANIC("not implemented");
    return 0;
}

uint32_t voh_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    // write to node. we cant write to the root node, but we can find underlying file and folder nodes

    PANIC("not implemented");
    return 0;
}

void voh_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

void voh_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

struct filesystem_node* voh_find_node_by_id(struct filesystem_node* fs_node, uint64_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct voh_objectdata* object_data = (struct voh_objectdata*)fs_node->filesystem_obj->object_data;
    //  kprintf("voh_find_node_by_id node id %llu of node %s\n", id, fs_node->name);
    if (fs_node == object_data->root_node) {
        /*
        * root node
        */
        if (0 == object_data->children) {
            //   kprintf("no chilren\n");
            return 0;
        } else {
            uint32_t child_count = arraylist_count(object_data->children);
            ASSERT(id < child_count);
            for (uint32_t i = 0; i < child_count; i++) {
                struct filesystem_node* n = (struct filesystem_node*)arraylist_get(object_data->children, i);
                //     kprintf("n %llu\n", n->id);
                if (n->id == id) {
                    return n;
                }
            }
            return 0;
        }
    } else {
        kprintf("voh nodes dont have chilren\n");
        return 0;
    }
}

void voh_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT_NOT_NULL(dir);
    struct voh_objectdata* object_data = (struct voh_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * root node
        */
        if (0 == object_data->children) {
            dir->count = 0;
        } else {
            dir->count = arraylist_count(object_data->children);
            for (uint32_t i = 0; i < dir->count; i++) {
                // node id is the index into the array list
                dir->ids[i] = i;
            }
        }
    } else {
        dir->count = 0;
        /*
        * return  here.  vohdev has no concept of folders, therefore every node
        * is at the top level; a child of the root
        */
    }
}

uint64_t voh_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    // voh nodes have no size
    return 0;
}

struct object* voh_attach(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &voh_init;
    objectinstance->uninit = &voh_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_VOH;
    objectinstance->object_data = 0;
    objectmgr_set_object_description(objectinstance, "VOH File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    api->close = &voh_close;
    api->find_id = &voh_find_node_by_id;
    api->open = &voh_open;
    api->root = &voh_get_root_node;
    api->write = &voh_write;
    api->read = &voh_read;
    api->list = &voh_list_directory;
    api->size = &voh_size;
    objectinstance->api = api;
    /*
     * device data
     */
    struct voh_objectdata* object_data = (struct voh_objectdata*)kmalloc(sizeof(struct voh_objectdata));
    object_data->root_node = filesystem_node_new(folder, objectinstance, name, 0, 0, 0);
    object_data->children = arraylist_new();
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
        kfree(object_data->children);
        kfree(object_data->root_node);
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void voh_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}

void voh_add_child(struct object* voh_device, struct filesystem_node* child_node) {
    ASSERT_NOT_NULL(voh_device);
    ASSERT_NOT_NULL(voh_device->object_data);
    ASSERT_NOT_NULL(child_node);
    struct voh_objectdata* object_data = (struct voh_objectdata*)voh_device->object_data;
    ASSERT_NOT_NULL(object_data);
    ASSERT_NOT_NULL(object_data->children);
    // the node id will be it's position in teh array
    child_node->id = arraylist_count(object_data->children);
    arraylist_add(object_data->children, child_node);
}

void voh_remove_child(struct object* voh_device, uint64_t id) {
    ASSERT_NOT_NULL(voh_device);
    ASSERT_NOT_NULL(voh_device->object_data);
    PANIC("not implemented");
}