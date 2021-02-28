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
#include <obj/logical/partition/partition.h>
#include <obj/logical/partition_table/mbr_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_part_table.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#define MBR_HEADER_LBA 0

uint8_t mbr_pt_part_table_total_partitions(struct object* dev);
uint64_t mbr_pt_part_table_get_partition_lba(struct object* dev, uint8_t partition);
uint64_t mbr_part_table_get_sector_count_function(struct object* dev, uint8_t partition);

struct mbr_pt_devicedata {
    struct object* block_device;
    uint32_t num_partitions;
} __attribute__((packed));

void mbr_pt_read_mbr_pt_header(struct object* dev, struct mbr_pt_header* header) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(header);
    blockutil_read(dev, (uint8_t*)header, sizeof(struct mbr_pt_header), MBR_HEADER_LBA, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t mbr_pt_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;

    object_data->num_partitions = mbr_pt_part_table_total_partitions(dev);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    if ((header.signature[0] == 0x55) && (header.signature[1] == 0xAA)) {
        kprintf("Init %s on %s (%s) with %llu partitions\n", dev->description, object_data->block_device->name,
                dev->name, object_data->num_partitions);

        // attach partitions
        fsutil_attach_partitions(dev);

        return 1;
    }
    return 0;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t mbr_pt_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, object_data->block_device->name, dev->name);

    /*
     * unmount partitions
     */
    fsutil_detach_partitions(dev);

    /*
     * done w device
     */
    kfree(dev->api);
    kfree(dev->object_data);
    return 1;
}

uint64_t mbr_pt_part_table_get_partition_lba(struct object* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    return header.partitions[partition].lba_start;
}

uint64_t mbr_part_table_get_sector_count_function(struct object* dev, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    return header.partitions[partition].total_sectors;
}

void mbr_pt_part_table_get_partition_type(struct object* dev, uint8_t partition, uint8_t* parititon_type,
                                          uint16_t len) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    ASSERT_NOT_NULL(parititon_type);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    ASSERT(partition < object_data->num_partitions);
    struct mbr_pt_header header;
    mbr_pt_read_mbr_pt_header(object_data->block_device, &header);
    uitoa3(header.partitions[partition].partition_type, parititon_type, len, 16);
}

uint8_t mbr_pt_part_table_total_partitions(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
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

uint8_t mbr_part_table_detachable(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;

    // check the partitions TODO
    return 1;
}

uint32_t mbr_part_read_sectors(struct object* dev, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                               uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(dev, partition_index);
    return blockutil_read(object_data->block_device, data, data_size, lba + start_lba, 0);
}

uint32_t mbr_part_write_sectors(struct object* dev, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                                uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    uint64_t lba = mbr_pt_part_table_get_partition_lba(dev, partition_index);
    return blockutil_write(object_data->block_device, data, data_size, lba + start_lba, 0);
}

struct object* mbr_pt_attach(struct object* block_device) {
    ASSERT(sizeof(struct mbr_pt_entry) == 16);
    ASSERT_NOT_NULL(block_device);
    ASSERT(1 == blockutil_is_block_device(block_device));
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &mbr_pt_init;
    deviceinstance->uninit = &mbr_pt_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION_TABLE;
    objectmgr_set_object_description(deviceinstance, "MBR partition table");
    /*
     * the device api
     */
    struct objecttype_part_table* api = (struct objecttype_part_table*)kmalloc(sizeof(struct objecttype_part_table));
    memzero((uint8_t*)api, sizeof(struct objecttype_part_table));
    api->partitions = &mbr_pt_part_table_total_partitions;
    api->lba = &mbr_pt_part_table_get_partition_lba;
    api->type = &mbr_pt_part_table_get_partition_type;
    api->sectors = &mbr_part_table_get_sector_count_function;
    api->detachable = &mbr_part_table_detachable;
    api->read = &mbr_part_read_sectors;
    api->write = &mbr_part_write_sectors;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)kmalloc(sizeof(struct mbr_pt_devicedata));
    object_data->block_device = block_device;
    object_data->num_partitions = 0;
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(block_device);
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

void mbr_pt_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct mbr_pt_devicedata* object_data = (struct mbr_pt_devicedata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}