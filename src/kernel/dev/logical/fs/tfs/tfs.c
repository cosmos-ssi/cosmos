//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/tfs/tfs.h>
#include <dev/logical/fs/tfs/tfs_block.h>
#include <dev/logical/fs/tfs/tfs_dir.h>
#include <dev/logical/fs/tfs/tfs_map.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

struct tfs_devicedata {
    struct object* partition_device;
} __attribute__((packed));

/*
 * format. I just guessed here.
 */
void tfs_format(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;

    /*
     * figure out how many map blocks we need
     */
    uint32_t number_map_blocks = tfs_map_block_count(device_data->partition_device);
    /*
     * create superblock
     */
    struct tfs_superblock_block superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct tfs_superblock_block));
    superblock.magic = TFS_MAGIC_SUPERBLOCK;
    superblock.blocks_size = (uint64_t)blockutil_get_sector_size(device_data->partition_device);
    superblock.blocks_count = (uint64_t)blockutil_get_sector_count(device_data->partition_device);
    superblock.number_map_blocks = number_map_blocks;
    superblock.root_dir = number_map_blocks + 1;  // sector one, since sector zero is the super-block
    kprintf("blocks_count %llu\n", superblock.blocks_count);
    kprintf("number_map_blocks %llu\n", superblock.number_map_blocks);
    kprintf("root_dir %llu\n", superblock.root_dir);
    /*
     * write superblock
     */
    tfs_write_superblock(device_data->partition_device, &superblock);
    /*
     * create & write map blocks
     */
    for (uint32_t i = 0; i < number_map_blocks; i++) {
        struct tfs_map_block map_block;
        memset((uint8_t*)&map_block, 0, sizeof(struct tfs_map_block));
        tfs_write_map_block(device_data->partition_device, &map_block, i + 1);
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
    tfs_write_dir_block(device_data->partition_device, &root_dir_block, superblock.root_dir);
    kprintf("dir block: %llu\n", superblock.root_dir);
}

bool tfs_dir_list_iterator(struct tfs_file_block* file_block) {
    ASSERT_NOT_NULL(file_block);
    kprintf("tfs file at block %s\n", file_block->name);
    return true;
}

struct fs_directory_listing* tfs_list_dir(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    tfs_dir_iterate_files(dev, &tfs_dir_list_iterator);
    return 0;
}

void tfs_read(struct object* dev, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < TFS_FILENAME_SIZE);
    ASSERT_NOT_NULL(dev->device_data);
    //   struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;
}

void tfs_write(struct object* dev, const uint8_t* name, const uint8_t* data, uint32_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(name);
    ASSERT_NOT_NULL(data);
    ASSERT(strlen(name) < TFS_FILENAME_SIZE);
    ASSERT_NOT_NULL(dev->device_data);
    //  struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;

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
uint8_t tfs_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tfs_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

struct object* tfs_attach(struct object* partition_device) {
    ASSERT(sizeof(struct tfs_superblock_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_dir_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_file_allocation_block) == TFS_BLOCK_SIZE);
    ASSERT(sizeof(struct tfs_map_block) == TFS_BLOCK_SIZE);
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_device();
    deviceinstance->init = &tfs_init;
    deviceinstance->uninit = &tfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    objectmgr_set_device_description(deviceinstance, "Trivial File System");
    /*
     * the device api
     */
    struct objecttype_filesystem* api = (struct objecttype_filesystem*)kmalloc(sizeof(struct objecttype_filesystem));
    memzero((uint8_t*)api, sizeof(struct objecttype_filesystem));
    //   api->format = &tfs_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct tfs_devicedata* device_data = (struct tfs_devicedata*)kmalloc(sizeof(struct tfs_devicedata));
    device_data->partition_device = partition_device;
    deviceinstance->device_data = device_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_device_refcount(partition_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(device_data);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void tfs_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tfs_devicedata* device_data = (struct tfs_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_device_refcount(device_data->partition_device);
    /*
    * detach
    */
    objectmgr_detach_device(dev);
}