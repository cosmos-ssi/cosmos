//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/swap/swap.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_swap.h>
#include <sys/string/mem.h>
#include <types.h>

struct swap_devicedata {
    struct object* block_device;
    uint16_t block_size;  // block size of the underlying physical device
};

/*
 * perform device instance specific init here
 */
uint8_t swap_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->block_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t swap_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, object_data->block_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);
    return 1;
}

uint16_t swap_block_size(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    return blockutil_get_sector_size(object_data->block_device);
}

void swap_read(struct object* dev, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    blockutil_read(object_data->block_device, data, swap_block_size(dev), block, 0);
}

void swap_write(struct object* dev, uint8_t* data, uint32_t block) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    blockutil_write(object_data->block_device, data, swap_block_size(dev), block, 0);
}

uint16_t swap_block_count(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    return blockutil_get_sector_count(object_data->block_device);
}

struct object* swap_attach(struct object* block_device) {
    ASSERT_NOT_NULL(block_device);
    ASSERT(1 == blockutil_is_block_device(block_device));
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &swap_init;
    deviceinstance->uninit = &swap_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = SWAP;
    objectmgr_set_object_description(deviceinstance, "Swap");
    /*
     * the device api
     */
    struct objecttype_swap* api = (struct objecttype_swap*)kmalloc(sizeof(struct objecttype_swap));
    memzero((uint8_t*)api, sizeof(struct objecttype_swap));
    api->write = &swap_write;
    api->read = &swap_read;
    api->block_count = &swap_block_count;
    api->block_size = &swap_block_size;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct swap_devicedata* object_data = (struct swap_devicedata*)kmalloc(sizeof(struct swap_devicedata));
    object_data->block_device = block_device;
    object_data->block_size = blockutil_get_sector_size(block_device);
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

void swap_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct swap_devicedata* object_data = (struct swap_devicedata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}
