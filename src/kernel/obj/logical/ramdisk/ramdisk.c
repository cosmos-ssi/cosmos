//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/ramdisk/ramdisk.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/objecttype/objecttype_block.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * ramdisk instance specific data
 */
struct ramdisk_objectdata {
    uint8_t* data;
    uint64_t size;
    uint16_t sector_size;
    uint16_t sector_count;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ramdisk_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;
    kprintf("Init %s of size %llu MB at %#hX (%s)\n", dev->description, (object_data->size) / 1024, object_data->data,
            dev->name);
    memset(object_data->data, 0, object_data->size);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ramdisk_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(object_data->data);
    kfree(object_data);
    kfree(dev->api);
    return 1;
}

uint8_t* ramdisk_calc_address(struct ramdisk_objectdata* object_data, uint32_t sector) {
    return (uint8_t*)(uint64_t)((object_data->data) + (sector * object_data->sector_size));
}

uint32_t ramdisk_read(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;

    // don't start off the end of the RAM
    ASSERT(start_lba < object_data->sector_count);

    // don't end off the end of the RAM
    uint32_t total_sectors = data_size / object_data->sector_size;
    if (0 != data_size % object_data->sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < object_data->sector_count);

    // copy
    uint8_t* block_address = ramdisk_calc_address(object_data, start_lba);
    memcpy(data, block_address, data_size);

    return data_size;
}

uint32_t ramdisk_write(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;

    // don't start off the end of the RAM
    ASSERT(start_lba < object_data->sector_count);

    // don't end off the end of the RAM
    uint32_t total_sectors = data_size / object_data->sector_size;
    if (0 != data_size % object_data->sector_size) {
        total_sectors += 1;
    }
    ASSERT((start_lba + total_sectors) < object_data->sector_count);

    // copy
    uint8_t* block_address = ramdisk_calc_address(object_data, start_lba);
    memcpy(block_address, data, (data_size));

    return data_size;
}

uint16_t ramdisk_sector_size(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;

    return object_data->sector_size;
}
uint32_t ramdisk_total_size(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)dev->object_data;

    return object_data->size;
}

struct object* ramdisk_attach(uint16_t sector_size, uint16_t sector_count) {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &ramdisk_init;
    deviceinstance->uninit = &ramdisk_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAMDISK;
    objectmgr_set_object_description(deviceinstance, "RAM disk");
    /*
     * device data
     */
    struct ramdisk_objectdata* object_data = (struct ramdisk_objectdata*)kmalloc(sizeof(struct ramdisk_objectdata));
    object_data->size = (uint64_t)sector_size * sector_count;
    object_data->sector_size = sector_size;
    object_data->sector_count = sector_count;
    object_data->data = kmalloc(object_data->size);
    memzero(object_data->data, object_data->size);
    deviceinstance->object_data = object_data;
    /*
     * the device api
     */
    struct objecttype_block* api = (struct objecttype_block*)kmalloc(sizeof(struct objecttype_block));
    memzero((uint8_t*)api, sizeof(struct objecttype_block));
    api->write = &ramdisk_write;
    api->read = &ramdisk_read;
    api->sector_size = &ramdisk_sector_size;
    api->total_size = &ramdisk_total_size;
    deviceinstance->api = api;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
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

void ramdisk_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    objectmgr_detach_object(dev);
}
