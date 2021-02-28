//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fs_util.h>
#include <obj/logical/fs/initrd/initrd.h>
#include <obj/logical/fs/node_cache.h>
#include <obj/logical/fs/node_util.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct initrd_file_header {
    uint8_t magic;
    uint8_t name[INITRD_NAME_SIZE];
    uint32_t offset;  // byte count from start of initrd
    uint32_t length;  // byte length
};

struct initrd_header {
    uint32_t number_files;
    struct initrd_file_header headers[INITRD_MAX_FILES];
};

struct initrd_objectdata {
    struct object* partition_objice;
    uint32_t lba;
    struct initrd_header header;
    struct filesystem_node* root_node;
    struct node_cache* nc;
};

/*
 * perform device instance specific init here
 */
uint8_t initrd_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)obj->object_data;
    /*
    * read the header
    */
    blockutil_read(object_data->partition_objice, (uint8_t*)&(object_data->header), sizeof(struct initrd_header),
                   object_data->lba, 0);

    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t initrd_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    kfree(obj->api);
    node_cache_delete(object_data->nc);
    kfree(object_data->root_node);
    kfree(object_data);
    return 1;
}

struct filesystem_node* initrd_get_root_node(struct object* filesystem_obj) {
    ASSERT_NOT_NULL(filesystem_obj);
    ASSERT_NOT_NULL(filesystem_obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)filesystem_obj->object_data;
    return object_data->root_node;
}

uint32_t initrd_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size) {
    //  kprintf("initrd_read node %s on device %s to buffer %#llX with length %llu\n", fs_node->name,
    //        fs_node->filesystem_obj->name, data, data_size);
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * cant read or write root node
        */
        return 0;
    } else {
        /*
        * get underlying sector size
        */
        uint32_t sector_size = blockutil_get_sector_size(object_data->partition_objice);

        uint32_t offset = object_data->header.headers[fs_node->id].offset;
        ASSERT(offset > 0);
        uint32_t length = object_data->header.headers[fs_node->id].length;
        ASSERT(data_size >= length);
        //  kprintf("offset %llu length %llu\n", offset, length);

        uint32_t lba_offset = offset / sector_size;
        uint32_t byte_offset = offset % sector_size;
        uint32_t total_sectors = length / sector_size;
        if ((length % 512) != 0) {
            total_sectors += 1;
        }
        // if it spans sectors
        if (byte_offset + length > sector_size) {
            total_sectors += 1;
        }
        uint32_t target_lba = object_data->lba + lba_offset;

        //   kprintf("lba_offset %llu byte_offset %llu total_sectors %llu target_lba %llu buffer_size %#llX\\n", lba_offset,
        //         byte_offset, total_sectors, target_lba, buffer_size);

        /*
        * read the blocks
        */
        blockutil_read(object_data->partition_objice, data, data_size, target_lba, byte_offset);

        return 1;
    }
}

uint32_t initrd_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    /*
    * cant write to initrd fs
    */
    return 0;
}

void initrd_open(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

void initrd_close(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);

    PANIC("not implemented");
}

struct filesystem_node* initrd_find_node_by_id(struct filesystem_node* fs_node, uint64_t id) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * root node
        */
        struct filesystem_node* ret = node_cache_find(object_data->nc, id);
        if (0 == ret) {
            ASSERT(id < object_data->header.number_files);
            // the node id is the index into the headers
            char* name = object_data->header.headers[id].name;
            ret = filesystem_node_new(file, fs_node->filesystem_obj, name, id, 0);
            node_cache_add(object_data->nc, ret);
        }
        return ret;
    } else {
        // devices are leaf nodes they have no children
        return 0;
    }
}

void initrd_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    ASSERT_NOT_NULL(dir);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * root node
        */
        dir->count = object_data->header.number_files;
        for (uint32_t i = 0; i < object_data->header.number_files; i++) {
            // node id is the index into the header table
            dir->ids[i] = i;
        }
    } else {
        dir->count = 0;
        // devices are leaf nodes they have no children
    }
}

uint64_t initrd_size(struct filesystem_node* fs_node) {
    ASSERT_NOT_NULL(fs_node);
    ASSERT_NOT_NULL(fs_node->filesystem_obj);
    ASSERT_NOT_NULL(fs_node->filesystem_obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)fs_node->filesystem_obj->object_data;
    if (fs_node == object_data->root_node) {
        /*
        * cant read or write root node
        */
        return 0;
    } else {
        return object_data->header.headers[fs_node->id].length;
    }
}

struct object* initrd_attach(struct object* partition_objice, uint32_t lba) {
    ASSERT_NOT_NULL(partition_objice);
    ASSERT(1 == blockutil_is_block_object(partition_objice));

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &initrd_init;
    objectinstance->uninit = &initrd_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "initrd File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    api->close = &initrd_close;
    api->find_id = &initrd_find_node_by_id;
    api->open = &initrd_open;
    api->root = &initrd_get_root_node;
    api->write = &initrd_write;
    api->read = &initrd_read;
    api->list = &initrd_list_directory;
    api->size = &initrd_size;
    objectinstance->api = api;
    /*
     * device data
     */
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)kmalloc(sizeof(struct initrd_objectdata));
    object_data->root_node = filesystem_node_new(folder, objectinstance, "initrd", 0, 0);
    object_data->lba = lba;
    object_data->partition_objice = partition_objice;
    object_data->nc = node_cache_new();
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_objice);
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

void initrd_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct initrd_objectdata* object_data = (struct initrd_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}

/*
* kernel is at 4th lba.  so figure out the kernel size and add
*/
uint64_t initrd_lba() {
    // currently the makefile just puts the initrd fs at offset 10MB in the boot disk
    return 20480;

    //linkage_show_kernel_section_data();
    //  uint64_t kernel_sector_count = linkage_get_kernel_sector_count();

    //  kprintf("kernel_sector_count %#llX\n", kernel_sector_count);
    // return 4 + kernel_sector_count;
}
