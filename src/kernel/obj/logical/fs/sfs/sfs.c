//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/sfs/sfs.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/mem.h>
#include <types.h>

#define SFS_VOLUME_IDENTIFIER 0x01
#define SFS_STARTING_MARKER 0x02
#define SFS_UNUSED_ENTRY 0x10
#define SFS_DIRECTORY_ENTRY 0x11
#define SFS_FILE_ENTRY 0x12
#define SFS_UNUSABLE_ENTRY 0x18
#define SFS_DELETED_DIRECTORY_ENTRY 0x19
#define SFS_DELETED_FILE_ENTRY 0x1A
#define SFS_CONTINUATION_ENTRY 0x20

struct sfs_superblock {
    uint8_t reserved1[11];
    uint8_t reserved2[21];
    uint8_t reserved3[372];
    uint64_t timestamp;
    uint64_t dataarea_size_blocks;
    uint64_t indexarea_size_bytes;
    uint8_t magic[3];
    uint8_t version;
    uint64_t total_blocks;
    uint32_t reserved_blocks;
    uint8_t block_size;
    uint8_t checksum;
    uint8_t reserved4[64];
    uint8_t reserved5[2];
} __attribute__((packed));

struct sfs_volume_identifier {
    uint8_t entry_type;
    uint8_t reserved[3];
    uint64_t timestamp;
    uint8_t name[52];
} __attribute__((packed));

struct sfs_starting_marker {
    uint8_t entry_type;
    uint8_t reserved[63];
} __attribute__((packed));

struct sfs_unused_entry {
    uint8_t entry_type;
    uint8_t reserved[63];
} __attribute__((packed));

struct sfs_directory_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint8_t reserved[54];
} __attribute__((packed));

struct sfs_file_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint64_t start_block;
    uint64_t end_block;
    uint64_t file_length;
    uint8_t name[30];
} __attribute__((packed));

struct sfs_unusable_entry {
    uint8_t entry_type;
    uint8_t reserved1[9];
    uint64_t start_block;
    uint64_t end_block;
    uint8_t reserved2[38];
} __attribute__((packed));

struct sfs_deleted_directory_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint8_t name[54];
} __attribute__((packed));

struct sfs_deleted_file_entry {
    uint8_t entry_type;
    uint8_t number_continuations;
    uint64_t timestamp;
    uint64_t start_block;
    uint64_t end_block;
    uint64_t file_length;
    uint8_t name[30];
} __attribute__((packed));

struct sfs_continuation_entry {
    uint8_t name[64];
} __attribute__((packed));

struct sfs_objectdata {
    struct object* partition_device;
} __attribute__((packed));

/*
 * check if valid superblock
 */
bool sfs_is_valid_superblock(struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(superblock);
    if ((superblock->magic[0] == 0x53) && (superblock->magic[1] == 0x46) && (superblock->magic[2] == 0x53)) {
        return true;
    }
    return false;
}

void sfs_read_superblock(struct object* dev, struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(dev);
    blockutil_read(dev, (uint8_t*)superblock, sizeof(struct sfs_superblock), 0, 0);
}

void sfs_format(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)dev->object_data;

    // device parameters
    //    uint64_t total_size = blockutil_get_total_size(object_data->block_device);
    uint32_t sector_size = blockutil_get_sector_size(object_data->partition_device);
    uint32_t total_sectors = blockutil_get_sector_count(object_data->partition_device);

    // create a superblock struct
    struct sfs_superblock superblock;
    memset((uint8_t*)&superblock, 0, sizeof(struct sfs_superblock));
    superblock.timestamp = 0;                             // later
    superblock.dataarea_size_blocks = total_sectors - 2;  // 1 for superblock and 1 for index
    superblock.indexarea_size_bytes = sector_size;        // 1 sector
    superblock.reserved_blocks = 1;                       // 1, for the superblock
    superblock.total_blocks = total_sectors;
    superblock.version = 0x10;  // 1.0
    superblock.magic[0] = 0x53;
    superblock.magic[1] = 0x46;
    superblock.magic[2] = 0x53;
    superblock.block_size = (sector_size / 512) + 1;

    // write superblock
    blockutil_write(object_data->partition_device, (uint8_t*)&superblock, sizeof(struct sfs_superblock), 0, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t sfs_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t sfs_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);

    struct sfs_objectdata* object_data = (struct sfs_objectdata*)dev->object_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, object_data->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);
    return 1;
}

struct object* sfs_attach(struct object* partition_device) {
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &sfs_init;
    deviceinstance->uninit = &sfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    objectmgr_set_object_description(deviceinstance, "Simple File System");
    /*
     * the device api
     */
    struct objecttype_filesystem* api = (struct objecttype_filesystem*)kmalloc(sizeof(struct objecttype_filesystem));
    memzero((uint8_t*)api, sizeof(struct objecttype_filesystem));
    //  api->format = &sfs_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)kmalloc(sizeof(struct sfs_objectdata));
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

void sfs_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}