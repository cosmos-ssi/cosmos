//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fat/fat.h>
#include <obj/logical/fs/fat/fat_support.h>
#include <obj/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <types.h>

// https://wiki.osdev.org/FAT

struct fat_objectdata {
    struct object* partition_object;
    struct fat_fs_parameters fs_parameters;
    struct filesystem_node* root_node;
};

/*
 * perform device instance specific init here
 */
uint8_t fat_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    object_data->root_node = filesystem_node_new(folder, obj, obj->name, 0, 0);
    fat_read_fs_parameters(object_data->partition_object, &(object_data->fs_parameters));
    fat_dump_fat_fs_parameters(&(object_data->fs_parameters));
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_object->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t fat_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_object->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

struct filesystem_node* fat_filesystem_get_root_node(struct object* filesystem_obj) {
    ASSERT_NOT_NULL(filesystem_obj);
    ASSERT_NOT_NULL(filesystem_obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)filesystem_obj->object_data;
    return object_data->root_node;
}

uint32_t fat_filesystem_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    PANIC("Not Implemented");
    return 0;
}

uint32_t fat_filesystem_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    PANIC("Not Implemented");
    return 0;
}

void fat_filesystem_open(struct filesystem_node* fs_node) {
    PANIC("Not Implemented");
}
void fat_filesystem_close(struct filesystem_node* fs_node) {}

struct filesystem_node* fat_filesystem_find_node_by_id(struct filesystem_node* fs_node, uint64_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    PANIC("Not Implemented");
    return 0;
}

void fat_filesystem_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)fs_node->filesystem_obj->object_data;

    if ((object_data->fs_parameters.type == FAT12) || (object_data->fs_parameters.type == FAT16)) {
        uint32_t current_sector = object_data->fs_parameters.first_root_dir_sector;

        bool more = true;
        while (more) {
            // read sector
            uint8_t* buffer = kmalloc(object_data->fs_parameters.sector_size);
            memset(buffer, 0, object_data->fs_parameters.sector_size);

            // kprintf("current_sector %llu\n", current_sector);

            // read first sector of root dir
            blockutil_read(object_data->partition_object, buffer, object_data->fs_parameters.sector_size,
                           current_sector, 0);

            // debug_show_memblock(buffer, object_data->fs_parameters.sector_size);

            // loop entries
            for (uint16_t i = 0; i < object_data->fs_parameters.sector_size; i = i + sizeof(struct fat_dir)) {
                struct fat_dir* entry = (struct fat_dir*)&(buffer[i]);
                // entry ok
                if (entry->name[0] != 0) {
                    // entry is used
                    if (entry->name[0] != 0xe5) {
                        // not a long file name
                        if (entry->name[10] != 0xFF) {

                            uint8_t fn[32];
                            fat_filename_from_fat(entry->name, fn, 32);

                            kprintf("fn %s\n", fn);
                            //		struct fs_directory* dir = (struct fs_directory*) kmalloc(sizeof(struct fs_directory));
                            //		dir->flags=entry->attributes;
                            //		memcpy(dir->name, entry->name,11);
                            //			dir->name[12]=0;

                            // dir->name = name;

                            //		dir->size = entry->size;
                            //		arraylist_add(ret->lst,dir);
                            //		kprintf("FF%s\n",dir->name);
                        }
                    }
                } else {
                    // we done!
                    more = false;
                    break;
                }
            }
            kfree(buffer);
            current_sector = current_sector + 1;
        }
    } else {
        PANIC("Unsupported FAT type");
    }
}

uint64_t fat_filesystem_size(struct filesystem_node* fs_node) {
    PANIC("Not Implemented");
    return 0;
}

struct object* fat_attach(struct object* partition_object) {
    ASSERT_NOT_NULL(partition_object);
    // basically the device needs to implement deviceapi_block
    ASSERT(1 == blockutil_is_block_object(partition_object));

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &fat_init;
    objectinstance->uninit = &fat_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "FAT File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    api->close = &fat_filesystem_close;
    api->find_id = &fat_filesystem_find_node_by_id;
    api->list = &fat_filesystem_list_directory;
    api->open = &fat_filesystem_open;
    api->read = &fat_filesystem_read;
    api->root = &fat_filesystem_get_root_node;
    api->size = &fat_filesystem_size;
    api->write = &fat_filesystem_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct fat_objectdata* object_data = (struct fat_objectdata*)kmalloc(sizeof(struct fat_objectdata));
    object_data->partition_object = partition_object;
    object_data->root_node = 0;
    objectinstance->object_data = object_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_object);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(object_data->root_node);
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void fat_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_object);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
