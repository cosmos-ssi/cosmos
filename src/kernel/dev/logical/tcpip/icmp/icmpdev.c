//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ip.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

struct icmp_devicedata {
    struct device* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t icmp_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->ethernet_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t icmp_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);

    return 1;
}

void icmp_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //  struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
}
void icmp_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //   struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
}

struct device* icmp_attach(struct device* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->devicetype == ETHERNET);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &icmp_init;
    deviceinstance->uninit = &icmp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ICMP;
    devicemgr_set_device_description(deviceinstance, "Internet Control Message Protocol");
    /*
     * the device api
     */
    struct deviceapi_ip* api = (struct deviceapi_ip*)kmalloc(sizeof(struct deviceapi_ip));
    memzero((uint8_t*)api, sizeof(struct deviceapi_ip));
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
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(ethernet_device);
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

void icmp_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct icmp_devicedata* device_data = (struct icmp_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->ethernet_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}