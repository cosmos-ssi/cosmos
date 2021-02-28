//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/fs_util.h>
#include <dev/logical/partition/partition.h>
#include <dev/logical/partition_table/partition_table_util.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_part_table.h>
#include <sys/string/mem.h>

struct partition_devicedata {
    struct object* partition_table_device;
    uint8_t type[64];  // type string
    uint8_t partition_index;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t partition_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;

    struct objecttype_part_table* pt_api = (struct objecttype_part_table*)device_data->partition_table_device->api;
    (*pt_api->type)(device_data->partition_table_device, device_data->partition_index, (device_data->type), 64);

    kprintf("Init %s on %s index %llu of type %s (%s)\n", dev->description, device_data->partition_table_device->name,
            device_data->partition_index, device_data->type, dev->name);

    // attach fs
    fsutil_attach_fs(dev);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t partition_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->partition_table_device->name, dev->name);
    // detach fs
    fsutil_detach_fs(dev);

    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

uint16_t partition_sector_size(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    return partition_table_util_sector_size(dev, device_data->partition_index);
}

uint32_t partition_total_size(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    return partition_table_util_total_size(dev, device_data->partition_index);
}

uint32_t partition_read_sectors(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    return partition_table_util_read_sectors(dev, device_data->partition_index, data, data_size, start_lba);
}

uint32_t partition_write_sectors(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    return partition_table_util_write_sectors(dev, device_data->partition_index, data, data_size, start_lba);
}

struct object* partition_attach(struct object* partition_table_device, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &partition_init;
    deviceinstance->uninit = &partition_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = PARTITION;
    objectmgr_set_object_description(deviceinstance, "Partition");
    /*
     * the device api
     */
    struct objecttype_block* api = (struct objecttype_block*)kmalloc(sizeof(struct objecttype_block));
    memzero((uint8_t*)api, sizeof(struct objecttype_block));
    api->sector_size = &partition_sector_size;
    api->total_size = &partition_total_size;
    api->read = &partition_read_sectors;
    api->write = &partition_write_sectors;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct partition_devicedata* device_data =
        (struct partition_devicedata*)kmalloc(sizeof(struct partition_devicedata));
    memzero((uint8_t*)device_data, sizeof(struct partition_devicedata));
    device_data->partition_table_device = partition_table_device;
    device_data->partition_index = partition_index;

    deviceinstance->device_data = device_data;

    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(partition_table_device);
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

void partition_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct partition_devicedata* device_data = (struct partition_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(device_data->partition_table_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}
