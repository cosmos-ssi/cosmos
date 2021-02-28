//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_ip.h>
#include <sys/string/mem.h>

struct icmp_devicedata {
    struct object* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t icmp_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t icmp_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);

    return 1;
}

void icmp_read(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //  struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
}
void icmp_write(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //   struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
}

struct object* icmp_attach(struct object* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_device();
    deviceinstance->init = &icmp_init;
    deviceinstance->uninit = &icmp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ICMP;
    objectmgr_set_device_description(deviceinstance, "Internet Control Message Protocol");
    /*
     * the device api
     */
    struct objecttype_ip* api = (struct objecttype_ip*)kmalloc(sizeof(struct objecttype_ip));
    memzero((uint8_t*)api, sizeof(struct objecttype_ip));
    api->read = &icmp_read;
    api->write = &icmp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct icmp_devicedata* device_data = (struct icmp_devicedata*)kmalloc(sizeof(struct icmp_devicedata));
    device_data->ethernet_device = ethernet_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_device_refcount(ethernet_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(device_data);
        kfree(deviceinstance);
        return 0;
    }
}

void icmp_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_device_refcount(device_data->ethernet_device);
    /*
    * detach
    */
    objectmgr_detach_device(dev);
}