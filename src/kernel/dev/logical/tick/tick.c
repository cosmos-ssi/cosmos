//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_pit.h>
#include <sys/deviceapi/deviceapi_tick.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

struct tick_devicedata {
    struct device* pit_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tick_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tick_devicedata* device_data = (struct tick_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->pit_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tick_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

uint64_t tick_read(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tick_devicedata* device_data = (struct tick_devicedata*)dev->device_data;

    struct deviceapi_pit* api = (struct deviceapi_pit*)device_data->pit_device->api;
    return (*api->tickcount)(device_data->pit_device);
}

struct device* tick_attach(struct device* pit_device) {
    ASSERT_NOT_NULL(pit_device);
    ASSERT(pit_device->devicetype == PIT);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &tick_init;
    deviceinstance->uninit = &tick_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = TICK;
    devicemgr_set_device_description(deviceinstance, "Tick Count");
    /*
     * the device api
     */
    struct deviceapi_tick* api = (struct deviceapi_tick*)kmalloc(sizeof(struct deviceapi_tick));
    memzero((uint8_t*)api, sizeof(struct deviceapi_tick));
    api->read = &tick_read;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct tick_devicedata* device_data = (struct tick_devicedata*)kmalloc(sizeof(struct tick_devicedata));
    device_data->pit_device = pit_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(pit_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void tick_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct tick_devicedata* device_data = (struct tick_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->pit_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}