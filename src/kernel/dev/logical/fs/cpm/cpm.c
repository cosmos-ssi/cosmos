//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/block_util.h>
#include <dev/logical/fs/cpm/cpm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

// https://obsolescence.wixsite.com/obsolescence/cpm-internals

#define CPM_MAX_FILES 64
#define CPM_FILE_ENTRY_LEN 32
#define CPM_SECTOR_SIZE 128
#define CPM_EMPTY 0xe5

struct cpm_file_entry {
    uint8_t used;
    uint8_t filename[8];
    uint8_t extension[3];
    uint8_t ex;
    uint8_t unused;
    uint8_t record_count;
    uint8_t disk_map[16];
    uint8_t cr;
    //   uint8_t dma[3];
} __attribute__((packed));

struct cpm_dir {
    struct cpm_file_entry files[CPM_MAX_FILES];
} __attribute__((packed));

struct cpm_devicedata {
    struct device* partition_device;
    uint16_t block_device_sector_size;
    uint8_t sectors_needed_for_dir;
    uint32_t byte_size_dir;
} __attribute__((packed));

void cpm_format(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)dev->device_data;

    /*
    * make the dir entry
    */
    struct cpm_dir dir;
    memset((uint8_t*)&dir, 0, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < CPM_MAX_FILES; i++) {
        dir.files[i].used = CPM_EMPTY;
    }

    /*
    * figure out how many sectors we need
    */
    uint8_t* buffer = kmalloc(device_data->byte_size_dir);
    memset(buffer, 0, device_data->byte_size_dir);
    memcpy(buffer, (uint8_t*)&dir, sizeof(struct cpm_dir));
    for (uint8_t i = 0; i < device_data->sectors_needed_for_dir; i++) {
        blockutil_write(device_data->partition_device, &(buffer[i]), device_data->byte_size_dir, i);
    }
    kfree(buffer);
}

void cpm_read_dir(struct device* dev, struct cpm_dir* dir) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)dev->device_data;

    uint8_t* buffer = kmalloc(device_data->byte_size_dir);
    memset(buffer, 0, device_data->byte_size_dir);
    for (uint8_t i = 0; i < device_data->sectors_needed_for_dir; i++) {
        blockutil_read(device_data->partition_device, &(buffer[i]), device_data->byte_size_dir, i);
    }

    // copy to dest
    memcpy((uint8_t*)dir, buffer, sizeof(struct cpm_dir));

    kfree(buffer);
}

/*
 * perform device instance specific init here
 */
uint8_t cpm_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t cpm_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->partition_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

struct device* cpm_attach(struct device* partition_device) {
    ASSERT(sizeof(struct cpm_file_entry) == CPM_FILE_ENTRY_LEN);
    ASSERT_NOT_NULL(partition_device);
    ASSERT(1 == blockutil_is_block_device(partition_device));
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &cpm_init;
    deviceinstance->uninit = &cpm_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = FILESYSTEM;
    devicemgr_set_device_description(deviceinstance, "CP/M File System");
    /*
     * the device api
     */
    struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)kmalloc(sizeof(struct deviceapi_filesystem));
    memzero((uint8_t*)api, sizeof(struct deviceapi_filesystem));
    // api->format = &cpm_format;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)kmalloc(sizeof(struct cpm_devicedata));
    device_data->partition_device = partition_device;
    device_data->block_device_sector_size = blockutil_get_sector_size(device_data->partition_device);
    // blocks needed for dir
    uint8_t sectors_needed = sizeof(struct cpm_dir) / device_data->block_device_sector_size;
    if (0 != (sizeof(struct cpm_dir) % device_data->block_device_sector_size)) {
        sectors_needed = sectors_needed + 1;
    }
    device_data->sectors_needed_for_dir = sectors_needed;
    device_data->byte_size_dir = device_data->sectors_needed_for_dir * device_data->block_device_sector_size;
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

void cpm_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct cpm_devicedata* device_data = (struct cpm_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->partition_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}
