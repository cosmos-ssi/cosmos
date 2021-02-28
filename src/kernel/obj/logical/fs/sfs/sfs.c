//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/sfs/sfs.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
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
    struct object* partition_objice;
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

void sfs_read_superblock(struct object* obj, struct sfs_superblock* superblock) {
    ASSERT_NOT_NULL(obj);
    blockutil_read(obj, (uint8_t*)superblock, sizeof(struct sfs_superblock), 0, 0);
}

void sfs_format(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)obj->object_data;

    // device parameters
    //    uint64_t total_size = blockutil_get_total_size(object_data->block_device);
    uint32_t sector_size = blockutil_get_sector_size(object_data->partition_objice);
    uint32_t total_sectors = blockutil_get_sector_count(object_data->partition_objice);

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
    blockutil_write(object_data->partition_objice, (uint8_t*)&superblock, sizeof(struct sfs_superblock), 0, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t sfs_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t sfs_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    struct sfs_objectdata* object_data = (struct sfs_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

struct object* sfs_attach(struct object* partition_objice) {
    ASSERT_NOT_NULL(partition_objice);
    ASSERT(1 == blockutil_is_block_object(partition_objice));
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &sfs_init;
    objectinstance->uninit = &sfs_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "Simple File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    //  api->format = &sfs_format;
    objectinstance->api = api;
    /*
     * device data
     */
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)kmalloc(sizeof(struct sfs_objectdata));
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

void sfs_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct sfs_objectdata* object_data = (struct sfs_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}