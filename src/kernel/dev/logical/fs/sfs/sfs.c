//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/sfs/sfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>

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

struct sfs_devicedata {
    struct device* partition_device;
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

void sfs_read_superblock(struct device* dev, struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(dev);
    blockutil_read(dev, (uint8_t*)superblock, sizeof(struct sfs_superblock), 0, 0);
}

void sfs_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct sfs_devicedata* device_data = (struct sfs_devicedata*)dev->device_data;

    // device parameters
    //    uint64_t total_size = blockutil_get_total_size(device_data->block_device);
    uint32_t sector_size = blockutil_get_sector_size(device_data->partition_device);
    uint32_t total_sectors = blockutil_get_sector_count(device_data->partition_device);

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
    blockutil_write(device_data->partition_device, (uint8_t*)&superblock, sizeof(struct sfs_superblock), 0, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t sfs_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct sfs_devicedata* device_data = (struct sfs_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t sfs_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    struct sfs_devicedata* device_data = (struct sfs_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

struct device* sfs_attach(struct device* partition_device) {
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &sfs_init;
    deviceinstance->uninit = &sfs_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "Simple File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    //  api->format = &sfs_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct sfs_devicedata* device_data = (struct sfs_devicedata*)kmalloc(sizeof(struct sfs_devicedata));
    device_data->partition_device = partition_device;
    deviceinstance->device_data = device_data;

    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(partition_device);

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

void sfs_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct sfs_devicedata* device_data = (struct sfs_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->partition_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}