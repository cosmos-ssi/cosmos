//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_rand.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

uint64_t RAND_MAX = (1 << 31);

struct rand_devicedata {
    uint64_t last;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t rand_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t rand_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}
/*
* https://rosettacode.org/wiki/Linear_congruential_generator#C
*/
uint64_t rand_read(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct rand_devicedata* device_data = (struct rand_devicedata*)dev->device_data;
    device_data->last = (device_data->last * 214013 + 2531011) & RAND_MAX >> 16;
    return device_data->last;
}

struct device* rand_attach() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &rand_init;
    deviceinstance->uninit = &rand_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAND;
    devicemgr_set_device_description(deviceinstance, "LCG Random Number Generator");
    /*
     * the device api
     */
    struct deviceapi_rand* api = (struct deviceapi_rand*)kmalloc(sizeof(struct deviceapi_rand));
    memzero((uint8_t*)api, sizeof(struct deviceapi_rand));
    api->read = &rand_read;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct rand_devicedata* device_data = (struct rand_devicedata*)kmalloc(sizeof(struct rand_devicedata));
    device_data->last = 7;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
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

void rand_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    devicemgr_detach_device(dev);
}