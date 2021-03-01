//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/cfs/cfs.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

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

struct cfs_objectdata {
    struct object* partition_objice;
} __attribute__((packed));

/*
 * total number of sectormap sectors for this disk
 */
uint32_t cfs_total_sectormap_sectors(struct object* obj) {
    uint32_t sectors = blockutil_get_sector_count(obj);

    if (SECTORS_MAPPED_PER_SECTOR >= sectors) {
        return 1;
    } else {
        return (sectors / SECTORS_MAPPED_PER_SECTOR) + 1;
    }
}

/*
 * read the superblock at lba 0
 */
void cfs_read_superblock(struct object* obj, struct cfs_superblock* superblock) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(superblock);
    blockutil_read(obj, (uint8_t*)superblock, sizeof(struct cfs_superblock), 0, 0);
}

/*
 * write the superblock at lba 0
 */
void cfs_write_superblock(struct object* obj, struct cfs_superblock* superblock) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(superblock);
    blockutil_write(obj, (uint8_t*)superblock, sizeof(struct cfs_superblock), 0, 0);
}

/*
 * write blockmap, at a certain sector
 */
void cfs_write_blockmap(struct object* obj, struct cfs_blockmap* blockmap, uint32_t sector) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(blockmap);
    blockutil_write(obj, (uint8_t*)blockmap, sizeof(struct cfs_blockmap), sector, 0);
}

/*
 * read blockmap, at a certain sector
 */
void cfs_read_blockmap(struct object* obj, struct cfs_blockmap* blockmap, uint32_t sector) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(blockmap);
    blockutil_read(obj, (uint8_t*)blockmap, sizeof(struct cfs_blockmap), sector, 0);
}

/*
 * format. I just guessed here.
 */
void cfs_format(struct object* obj) {
    ASSERT_NOT_NULL(obj->object_data);
    struct cfs_objectdata* object_data = (struct cfs_objectdata*)obj->object_data;

    uint32_t total_sectors_blockmap = cfs_total_sectormap_sectors(object_data->partition_objice);
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
    cfs_write_superblock(object_data->partition_objice, &superblock);
    /*
     * blockmaps.  first one at lba 1.
     */
    for (uint32_t i = 0; i < total_sectors_blockmap; i++) {
        struct cfs_blockmap blockmap;
        memset((uint8_t*)&blockmap, 0, sizeof(struct cfs_superblock));
        cfs_write_blockmap(object_data->partition_objice, &blockmap, 1 + i);
    }
}

/*
 * perform device instance specific init here
 */
uint8_t cfs_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cfs_objectdata* object_data = (struct cfs_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t cfs_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cfs_objectdata* object_data = (struct cfs_objectdata*)obj->object_data;

    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->partition_objice->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

struct object* cfs_attach(struct object* partition_objice) {
    ASSERT_NOT_NULL(partition_objice);
    ASSERT(1 == blockutil_is_block_object(partition_objice));

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &cfs_init;
    objectinstance->uninit = &cfs_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_FILESYSTEM;
    objectmgr_set_object_description(objectinstance, "Cosmos File System");
    /*
     * the device api
     */
    struct objectinterface_filesystem* api =
        (struct objectinterface_filesystem*)kmalloc(sizeof(struct objectinterface_filesystem));
    memzero((uint8_t*)api, sizeof(struct objectinterface_filesystem));
    //  api->format = &cfs_format;
    objectinstance->api = api;
    /*
     * device data
     */
    struct cfs_objectdata* object_data = (struct cfs_objectdata*)kmalloc(sizeof(struct cfs_objectdata));
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

void cfs_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct cfs_objectdata* object_data = (struct cfs_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->partition_objice);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}