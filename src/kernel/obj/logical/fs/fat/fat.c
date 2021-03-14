//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fat/fat.h>
#include <obj/logical/fs/fat/fat_support.h>
#include <obj/logical/fs/filesystem_node_map.h>
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
#include <sys/string/string.h>
#include <types.h>

/*
* the filesystem_node field "node_data" field for FAT folder objects contains the starting cluster
*/

// https://wiki.osdev.org/FAT

struct fat_objectdata {
    struct object* block_object;
    struct fat_fs_parameters fs_parameters;
    struct filesystem_node* root_node;
    struct filesystem_node_map* filesystem_nodes;
    uint64_t next_filesystem_node_id;
};

/*
 * perform device instance specific init here
 */
uint8_t fat_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    fat_read_fs_parameters(object_data->block_object, &(object_data->fs_parameters));
    uint64_t first_dir_sector = (uint64_t)object_data->fs_parameters.first_root_dir_sector;
    object_data->root_node = filesystem_node_new(folder, obj, obj->name, 0, object_data->next_filesystem_node_id,
                                                 (void*)first_dir_sector, 0);
    object_data->next_filesystem_node_id += 1;
    fat_dump_fat_fs_parameters(&(object_data->fs_parameters));
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->block_object->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t fat_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->block_object->name, obj->name);
    filesystem_node_map_clear(object_data->filesystem_nodes);
    filesystem_node_map_delete(object_data->filesystem_nodes);
    kfree(object_data->root_node);
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
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT(data_size >= fs_node->size);
    uint64_t start_sector = (uint64_t)fs_node->node_data;
    struct fat_objectdata* object_data = (struct fat_objectdata*)fs_node->filesystem_obj->object_data;

    return blockutil_read(object_data->block_object, data, data_size, start_sector, 0);
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

    struct fat_objectdata* object_data = (struct fat_objectdata*)fs_node->filesystem_obj->object_data;
    //  kprintf("finding node id: %llu\n", id);
    return filesystem_node_map_find_id(object_data->filesystem_nodes, id);
}

void fat_filesystem_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)fs_node->filesystem_obj->object_data;

    if (fs_node->type == folder) {
        //    kprintf("listing folder %s\n", fs_node->name);

        dir->count = 0;
        if ((object_data->fs_parameters.type == FAT12) || (object_data->fs_parameters.type == FAT16)) {
            uint64_t current_sector = (uint64_t)fs_node->node_data;
            //      kprintf("cs: %llu\n", current_sector);

            bool more = true;
            while (more) {
                // read sector
                uint8_t* buffer = kmalloc(object_data->fs_parameters.sector_size);
                memset(buffer, 0, object_data->fs_parameters.sector_size);

                // read first sector of root dir
                blockutil_read(object_data->block_object, buffer, object_data->fs_parameters.sector_size,
                               current_sector, 0);

                // loop entries
                for (uint16_t i = 0; i < object_data->fs_parameters.sector_size; i = i + sizeof(struct fat_dir_entry)) {
                    struct fat_dir_entry* entry = (struct fat_dir_entry*)&(buffer[i]);

                    // entry ok
                    if (entry->name[0] != 0) {
                        // entry is used
                        if (entry->name[0] != 0xe5) {
                            // not a long file name
                            if (entry->name[10] != 0xFF) {

                                // get name
                                uint8_t fn[32];
                                fat_filename_from_fat(entry->name, fn, 32);
                                tolower(fn);
                                //      kprintf("name: %s\n", fn);
                                if ((0 != strcmp(fn, ".")) && (0 != strcmp(fn, ".."))) {

                                    //       kprintf("fn %s\n", fn);
                                    uint64_t node_id = filesystem_node_map_find_name(object_data->filesystem_nodes, fn);

                                    uint32_t node_sector =
                                        ((entry->cluster_low - 2) * object_data->fs_parameters.sectors_per_cluster) +
                                        object_data->fs_parameters.first_data_sector;

                                    if (0 == node_id) {
                                        struct filesystem_node* node = 0;
                                        if (entry->attributes & FAT_DIRECTORY) {
                                            //                    kprintf("new folder %s at sector %llu\n", fn, node_sector);

                                            node = filesystem_node_new(folder, fs_node->filesystem_obj, fn, entry->size,
                                                                       object_data->next_filesystem_node_id,
                                                                       (void*)(uint64_t)node_sector, fs_node->id);
                                        } else if (0 == (entry->attributes & FAT_IGNORE)) {
                                            //                  kprintf("new file %s at sector %llu\n", fn, node_sector);

                                            node = filesystem_node_new(file, fs_node->filesystem_obj, fn, entry->size,
                                                                       object_data->next_filesystem_node_id,
                                                                       (void*)(uint64_t)node_sector, fs_node->id);
                                        }
                                        if (0 != node) {

                                            //                                            uint8_t node_full_name[FILESYSTEM_MAX_PATH];
                                            //                                           filesystem_node_map_get_node_name(object_data->filesystem_nodes, node,
                                            //                                                                           node_full_name, FILESYSTEM_MAX_PATH);

                                            object_data->next_filesystem_node_id += 1;
                                            filesystem_node_map_insert(object_data->filesystem_nodes, node);
                                            //           kprintf("new node %llu\n", node->id);
                                            node_id = node->id;
                                        }
                                    }
                                    if (0 != node_id) {
                                        //     kprintf("count %llu, id %llu\n", dir->count, node_id);
                                        dir->ids[dir->count] = node_id;
                                        dir->count += 1;
                                    }
                                }
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
    } else {
        dir->count = 0;
    }
}

struct object* fat_attach(struct object* block_object) {
    ASSERT_NOT_NULL(block_object);
    // basically the device needs to implement deviceapi_block
    ASSERT(1 == blockutil_is_block_object(block_object));

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
    api->write = &fat_filesystem_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct fat_objectdata* object_data = (struct fat_objectdata*)kmalloc(sizeof(struct fat_objectdata));
    object_data->block_object = block_object;
    object_data->root_node = 0;
    object_data->filesystem_nodes = filesystem_node_map_new();
    object_data->next_filesystem_node_id = 1;
    objectinstance->object_data = object_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(block_object);
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

void fat_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct fat_objectdata* object_data = (struct fat_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_object);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
