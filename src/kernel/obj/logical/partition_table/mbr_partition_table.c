//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Partition_Table
// https://wiki.osdev.org/MBR_(x86)
// https://en.wikipedia.org/wiki/Master_boot_record

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fs_util.h>
#include <obj/logical/partition_table/mbr_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_part_table.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#define MBR_HEADER_LBA 0

uint8_t mbr_pt_part_table_total_partitions(struct object* obj);
uint64_t mbr_pt_part_table_get_partition_lba(struct object* obj, uint8_t partition);
uint64_t mbr_part_table_get_sector_count_function(struct object* obj, uint8_t partition);

struct mbr_pt_objectdata {
    struct object* block_device;
    uint32_t num_partitions;
} __attribute__((packed));

void mbr_pt_read_mbr_pt_header(struct object* obj, struct mbr_pt_header* header) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(header);
    blockutil_read(obj, (uint8_t*)header, sizeof(struct mbr_pt_header), MBR_HEADER_LBA, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t mbr_pt_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;

    object_data->num_partitions = mbr_pt_part_table_total_partitions(obj);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    if ((header.signature[0] == 0x55) && (header.signature[1] == 0xAA)) {
        kprintf("Init %s on %s (%s) with %llu partitions\n", obj->description, object_data->block_device->name,
                obj->name, object_data->num_partitions);

        // attach partitions
        fsutil_attach_partitions(obj);

        return 1;
    }
    return 0;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t mbr_pt_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->block_device->name, obj->name);

    /*
     * unmount partitions
     */
    fsutil_detach_partitions(obj);

    /*
     * done w device
     */
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

uint64_t mbr_pt_part_table_get_partition_lba(struct object* obj, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    return header.partitions[partition].lba_start;
}

uint64_t mbr_part_table_get_sector_count_function(struct object* obj, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    return header.partitions[partition].total_sectors;
}

void mbr_pt_part_table_get_partition_type(struct object* obj, uint8_t partition, uint8_t* parititon_type,
                                          uint16_t len) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(parititon_type);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    uitoa3(header.partitions[partition].partition_type, parititon_type, len, 16);
}

uint8_t mbr_pt_part_table_total_partitions(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);

    uint8_t ret = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (header.partitions[i].lba_start != 0) {
            ret = ret + 1;
        }
    }
    return ret;
}

uint8_t mbr_part_table_detachable(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;

    // check the partitions TODO
    return 1;
}

uint32_t mbr_part_read_sectors(struct object* obj, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                               uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(obj, partition_index);
    return blockutil_read(object_data->block_device, data, data_size, lba + start_lba, 0);
}

uint32_t mbr_part_write_sectors(struct object* obj, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                                uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(obj, partition_index);
    return blockutil_write(object_data->block_device, data, data_size, lba + start_lba, 0);
}

struct object* mbr_pt_attach(struct object* block_device) {
    ASSERT(sizeof(struct mbr_pt_entry) == 16);
    ASSERT_NOT_NULL(block_device);
    ASSERT(1 == blockutil_is_block_object(block_device));
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &mbr_pt_init;
    objectinstance->uninit = &mbr_pt_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = PARTITION_TABLE;
    objectmgr_set_object_description(objectinstance, "MBR partition table");
    /*
     * the device api
     */
    struct objectinterface_part_table* api =
        (struct objectinterface_part_table*)kmalloc(sizeof(struct objectinterface_part_table));
    memzero((uint8_t*)api, sizeof(struct objectinterface_part_table));
    api->partitions = &mbr_pt_part_table_total_partitions;
    api->lba = &mbr_pt_part_table_get_partition_lba;
    api->type = &mbr_pt_part_table_get_partition_type;
    api->sectors = &mbr_part_table_get_sector_count_function;
    api->detachable = &mbr_part_table_detachable;
    api->read = &mbr_part_read_sectors;
    api->write = &mbr_part_write_sectors;
    objectinstance->api = api;
    /*
     * device data
     */
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)kmalloc(sizeof(struct mbr_pt_objectdata));
    object_data->block_device = block_device;
    object_data->num_partitions = 0;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(block_device);
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

void mbr_pt_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct mbr_pt_objectdata* object_data = (struct mbr_pt_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}