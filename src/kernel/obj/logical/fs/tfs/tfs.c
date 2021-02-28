//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/tfs/tfs.h>
#include <obj/logical/fs/tfs/tfs_block.h>
#include <obj/logical/fs/tfs/tfs_dir.h>
#include <obj/logical/fs/tfs/tfs_map.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <types.h>

struct tfs_objectdata {
    struct object* partition_objice;
} __attribute__((packed));

/*
 * format. I just guessed here.
 */
void tfs_format(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;

    /*
     * figure out how many map blocks we need
     */
    uint32_t number_map_blocks = tfs_map_block_count(object_data->partition_objice);
    /*
     * create superblock
     */
    struct tfs_superblock_block superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct tfs_superblock_block));
    superblock.magic = TFS_MAGIC_SUPERBLOCK;
    superblock.blocks_size = (uint64_t)blockutil_get_sector_size(object_data->partition_objice);
    superblock.blocks_count = (uint64_t)blockutil_get_sector_count(object_data->partition_objice);
    superblock.number_map_blocks = number_map_blocks;
    superblock.root_dir = number_map_blocks + 1;  // sector one, since sector zero is the super-block
    kprintf("blocks_count %llu\n", superblock.blocks_count);
    kprintf("number_map_blocks %llu\n", superblock.number_map_blocks);
    kprintf("root_dir %llu\n", superblock.root_dir);
    /*
     * write superblock
     */
    tfs_write_superblock(object_data->partition_objice, &superblock);
    /*
     * create & write map blocks
     */
    for (uint32_t i = 0; i < number_map_blocks; i++) {
        struct tfs_map_block map_block;
        memset((uint8_t*)&map_block, 0, sizeof(struct tfs_map_block));
        tfs_write_map_block(object_data->partition_objice, &map_block, i + 1);
        //    kprintf("map block: %llu\n",i+1);
    }
    /*
     * root dir block
     */
    struct tfs_dir_block root_dir_block;
    memset((uint8_t*)&root_dir_block, 0, sizeof(struct tfs_dir_block));
    root_dir_block.next = 0;
    /*
     * write root dir
     */
    tfs_write_dir_block(object_data->partition_objice, &root_dir_block, superblock.root_dir);
    kprintf("dir block: %llu\n", superblock.root_dir);
}

bool tfs_dir_list_iterator(struct tfs_file_block* file_block) {
    ASSERT_NOT_NULL(file_block);
    kprintf("tfs file at block %s\n", file_block->name);
    return true;
}

struct fs_directory_listing* tfs_list_dir(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    tfs_dir_iterate_files(obj, &tfs_dir_list_iterator);
    return 0;
}

void tfs_read(struct object* obj, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < TFS_FILENAME_SIZE);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;
}

void tfs_write(struct object* obj, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < TFS_FILENAME_SIZE);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;

    kprintf("write file: %s of length %llu\n", name, size);
    /*
     * find an empty spot in the root dir
     */

    /*
     * write to the dir
     */

    /*
     * make an allocation block
     */
}

/*
 * perform device instance specific init here
 */
uint8_t tfs_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tfs_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

struct object* tfs_attach(struct object* partition_objice) {
    ASSERT(sizeof(struct tfs_superblock_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_dir_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_allocation_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_map_block) == TFS_BLOCK_SIZE);
    ASSERT_NOT_NULL(partition_objice);
    ASSERT(1 == blockutil_is_block_object(partition_objice));
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &tfs_init;
    objectinstance->uninit = &tfs_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "Trivial File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    //   api->format = &tfs_format;
    objectinstance->api = api;
    /*
     * device data
     */
    struct tfs_objectdata* object_data = (struct tfs_objectdata*)kmalloc(sizeof(struct tfs_objectdata));
    object_data->partition_objice = partition_objice;
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
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void tfs_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tfs_objectdata* object_data = (struct tfs_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}