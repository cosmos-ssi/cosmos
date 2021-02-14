//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/ramdisk/ramdisk.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * ramdisk instance specific data
 */
struct ramdisk_devicedata {
    uint8_t* data;
    uint64_t size;
    uint16_t sector_size;
    uint16_t sector_count;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ramdisk_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;
    kprintf("Init %s of size %llu MB at %#hX (%s)\n", dev->description, (device_data->size) / 1024, device_data->data,
            dev->name);
    memset(device_data->data, 0, device_data->size);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ramdisk_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(device_data->data);
    kfree(device_data);
    kfree(dev->api);
    return 1;
}

uint8_t* ramdisk_calc_address(struct ramdisk_devicedata* device_data, uint32_t sector) {
    return (uint8_t*)(uint64_t)((device_data->data) + (sector * device_data->sector_size));
}

uint32_t ramdisk_read(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;

    // don't start off the end of the RAM
    ASSERT(start_lba < device_data->sector_count);

    // don't end off the end of the RAM
    uint32_t total_sectors = data_size / device_data->sector_size;
    if (0 != data_size % device_data->sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < device_data->sector_count);

    // copy
    uint8_t* block_address = ramdisk_calc_address(device_data, start_lba);
    memcpy(data, block_address, data_size);

    return data_size;
}

uint32_t ramdisk_write(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;

    // don't start off the end of the RAM
    ASSERT(start_lba < device_data->sector_count);

    // don't end off the end of the RAM
    uint32_t total_sectors = data_size / device_data->sector_size;
    if (0 != data_size % device_data->sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < device_data->sector_count);

    // copy
    uint8_t* block_address = ramdisk_calc_address(device_data, start_lba);
    memcpy(block_address, data, (data_size));

    return data_size;
}

uint16_t ramdisk_sector_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;

    return device_data->sector_size;
}
uint32_t ramdisk_total_size(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)dev->device_data;

    return device_data->size;
}

struct device* ramdisk_attach(uint16_t sector_size, uint16_t sector_count) {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &ramdisk_init;
    deviceinstance->uninit = &ramdisk_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAMDISK;
    devicemgr_set_device_description(deviceinstance, "RAM disk");
    /*
     * device data
     */
    struct ramdisk_devicedata* device_data = (struct ramdisk_devicedata*)kmalloc(sizeof(struct ramdisk_devicedata));
    device_data->size = (uint64_t)sector_size * sector_count;
    device_data->sector_size = sector_size;
    device_data->sector_count = sector_count;
    device_data->data = kmalloc(device_data->size);
    memzero(device_data->data, device_data->size);
    deviceinstance->device_data = device_data;
    /*
     * the device api
     */
    struct deviceapi_block* api = (struct deviceapi_block*)kmalloc(sizeof(struct deviceapi_block));
    memzero((uint8_t*)api, sizeof(struct deviceapi_block));
    api->write = &ramdisk_write;
    api->read = &ramdisk_read;
    api->sector_size = &ramdisk_sector_size;
    api->total_size = &ramdisk_total_size;
    deviceinstance->api = api;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
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

void ramdisk_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}
