//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/cfs/cfs.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/mem.h>

/*
 * every 512 byte sector contains 512 bytes of sector map
 */
#define SECTORMAP_BYTE_PER_SECTORS 512
/*
 * 1 bit per sector
 */
#define SECTORS_MAPPED_PER_SECTOR SECTORMAP_BYTE_PER_SECTORS * 8

#define CFS_MAGIC_SUPERBLOCK 0x303244594C4F4647
#define CFS_MAGIC_DATA_DIR 0x30324D4C424D4C42
#define CFS_MAGIC_PRESENTATION_DIR 0x4E45444942454F4A
#define CFS_MAGIC_GROUP_DIR 0x535241484E444942
#define CFS_MAGIC_DATA 0x30324D4C424D4C00
#define CFS_MAGIC_PRESENTATION 0x4E45444942454F00
#define CFS_MAGIC_GROUP 0x535241484E444900
#define CFS_MAGIC_BLOCKMAP 0x535241484E444907

struct cfs_superblock {
    uint64_t magic;  // CFS_MAGIC_SUPERBLOCK
    uint64_t lastmount;
    uint64_t primary_data_space;
    uint64_t primary_presentation_space;
    uint64_t primary_group_directory;
    uint64_t reserved[58];
} __attribute__((packed));

struct cfs_directory {
    uint64_t magic;  // CFS_MAGIC_DATA_DIR | CFS_MAGIC_PRESENTATION_DIR | CFS_MAGIC_GROUP_DIR
    uint64_t previous;
    uint64_t next;
    uint64_t pointer[61];
} __attribute__((packed));

struct cfs_data {
    uint64_t magic;  // CFS_MAGIC_DATA
} __attribute__((packed));

struct cfs_presentation {
    uint64_t magic;  // CFS_MAGIC_PRESENTATION
} __attribute__((packed));

struct cfs_groups {
    uint64_t magic;  // CFS_MAGIC_GROUP
} __attribute__((packed));

/*
 * bit map of sector status (0 = unused, 1 = used)
 */
struct cfs_blockmap {
    uint64_t magic;         // CFS_MAGIC_BLOCKMAP
    uint8_t map[512 - 64];  // 448 bytes of bitmap, mapping 448*8=3584 sectors
} __attribute__((packed));

struct cfs_devicedata {
    struct object* partition_device;
} __attribute__((packed));

/*
 * total number of sectormap sectors for this disk
 */
uint32_t cfs_total_sectormap_sectors(struct object* dev) {
    uint32_t sectors = blockutil_get_sector_count(dev);

    if (SECTORS_MAPPED_PER_SECTOR >= sectors) {
        return 1;
    } else {
        return (sectors / SECTORS_MAPPED_PER_SECTOR) + 1;
    }
}

/*
 * read the superblock at lba 0
 */
void cfs_read_superblock(struct object* dev, struct cfs_superblock* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    blockutil_read(dev, (uint8_t*)superblock, sizeof(struct cfs_superblock), 0, 0);
}

/*
 * write the superblock at lba 0
 */
void cfs_write_superblock(struct object* dev, struct cfs_superblock* superblock) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(superblock);
    blockutil_write(dev, (uint8_t*)superblock, sizeof(struct cfs_superblock), 0, 0);
}

/*
 * write blockmap, at a certain sector
 */
void cfs_write_blockmap(struct object* dev, struct cfs_blockmap* blockmap, uint32_t sector) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(blockmap);
    blockutil_write(dev, (uint8_t*)blockmap, sizeof(struct cfs_blockmap), sector, 0);
}

/*
 * read blockmap, at a certain sector
 */
void cfs_read_blockmap(struct object* dev, struct cfs_blockmap* blockmap, uint32_t sector) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(blockmap);
    blockutil_read(dev, (uint8_t*)blockmap, sizeof(struct cfs_blockmap), sector, 0);
}

/*
 * format. I just guessed here.
 */
void cfs_format(struct object* dev) {
    ASSERT_NOT_NULL(dev->object_data);
    struct cfs_devicedata* object_data = (struct cfs_devicedata*)dev->object_data;

    uint32_t total_sectors_blockmap = cfs_total_sectormap_sectors(object_data->partition_device);
    // kprintf("Blockmap sectors %llu\n",total_sectors_blockmap);
    /*
     * superblock
     */
    struct cfs_superblock superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct cfs_superblock));
    superblock.magic = CFS_MAGIC_SUPERBLOCK;
    superblock.lastmount = 0;
    superblock.primary_data_space = 2;
    superblock.primary_presentation_space = 3;
    superblock.primary_group_directory = 4;
    cfs_write_superblock(object_data->partition_device, &superblock);
    /*
     * blockmaps.  first one at lba 1.
     */
    for (uint32_t i = 0; i < total_sectors_blockmap; i++) {
        struct cfs_blockmap blockmap;
        memset((uint8_t*)&blockmap, 0, sizeof(struct cfs_superblock));
        cfs_write_blockmap(object_data->partition_device, &blockmap, 1 + i);
    }
}

/*
 * perform device instance specific init here
 */
uint8_t cfs_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct cfs_devicedata* object_data = (struct cfs_devicedata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t cfs_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct cfs_devicedata* object_data = (struct cfs_devicedata*)dev->object_data;

    kprintf("Uninit %s on %s (%s)\n", dev->description, object_data->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);
    return 1;
}

struct object* cfs_attach(struct object* partition_device) {
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &cfs_init;
    deviceinstance->uninit = &cfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    objectmgr_set_object_description(deviceinstance, "Cosmos File System");
    /*
     * the device api
     */
    struct objecttype_filesystem* api = (struct objecttype_filesystem*)kmalloc(sizeof(struct objecttype_filesystem));
    memzero((uint8_t*)api, sizeof(struct objecttype_filesystem));
    //  api->format = &cfs_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct cfs_devicedata* object_data = (struct cfs_devicedata*)kmalloc(sizeof(struct cfs_devicedata));
    object_data->partition_device = partition_device;
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(object_data);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void cfs_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct cfs_devicedata* object_data = (struct cfs_devicedata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}