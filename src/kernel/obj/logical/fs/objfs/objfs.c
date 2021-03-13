//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/filesystem_node_map.h>
#include <obj/logical/fs/node_util.h>
#include <obj/logical/fs/objfs/objfs.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objectregistry/objectregistry.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <types.h>

/*
* node ids are the object handles
*/
/*
* object data.
*/
struct objfs_objectdata {
    struct filesystem_node* root_node;
    struct filesystem_node_map* filesystem_nodes;
    uint64_t next_filesystem_node_id;
};

/*
 * perform device instance specific init here
 */
uint8_t objfs_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)obj->object_data;
    object_data->root_node = filesystem_node_new(folder, obj, obj->name, object_data->next_filesystem_node_id, 0, 0);
    object_data->next_filesystem_node_id += 1;

    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t objfs_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s  (%s)\n", obj->description, obj->name);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)obj->object_data;
    filesystem_node_map_clear(object_data->filesystem_nodes);
    filesystem_node_map_delete(object_data->filesystem_nodes);
    kfree(obj->api);
    kfree(object_data->root_node);
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
    //   kprintf("finding node id: %llu\n", id);
    struct filesystem_node* ret = filesystem_node_map_find_id(object_data->filesystem_nodes, id);
    if (0 != ret) {
        //       kprintf("node %llu has name %s\n", id, ret->name);
    }
    return ret;
}

void objfs_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT_NOT_NULL(dir);
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)fs_node->filesystem_obj->object_data;
    /*
    * root node
    */
    if (fs_node == object_data->root_node) {
        dir->count = 0;
        for (uint32_t i = 0; i < objecttypes_count(); i++) {
            struct object_type* ot = objecttypes_get(i);
            ASSERT_NOT_NULL(ot);
            if (0 != ot) {
                for (uint32_t j = 0; j < objectregistry_objectcount_type(ot->id); j++) {
                    struct object* obj = objectregistry_get_object(ot->id, j);
                    uint64_t node_id = filesystem_node_map_find_name(object_data->filesystem_nodes, obj->name);
                    if (0 == node_id) {
                        // object_data is the obhect handle
                        struct filesystem_node* node =
                            filesystem_node_new(file, fs_node->filesystem_obj, obj->name,
                                                object_data->next_filesystem_node_id, (void*)obj->handle, 0);
                        object_data->next_filesystem_node_id += 1;
                        filesystem_node_map_insert(object_data->filesystem_nodes, node);
                        kprintf("new node %llu\n", node->id);
                        node_id = node->id;
                    }
                    //     kprintf("count %llu, id %llu\n", dir->count, node_id);
                    dir->ids[dir->count] = node_id;
                    dir->count += 1;
                }
            } else {
                dir->count = 0;
                // devices are leaf nodes they have no children
            }
        }
    } else {
        dir->count = 0;
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
    struct object* objectinstance = object_new_object();
    objectinstance->init = &objfs_init;
    objectinstance->uninit = &objfs_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_OBJFS;
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
     * object data
     */
    struct objfs_objectdata* object_data = (struct objfs_objectdata*)kmalloc(sizeof(struct objfs_objectdata));
    object_data->root_node = 0;
    object_data->next_filesystem_node_id = 1;
    object_data->filesystem_nodes = filesystem_node_map_new();
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
        filesystem_node_map_clear(object_data->filesystem_nodes);
        filesystem_node_map_delete(object_data->filesystem_nodes);
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
