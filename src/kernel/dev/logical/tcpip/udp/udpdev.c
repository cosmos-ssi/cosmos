//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_tcp.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

struct udp_devicedata {
    struct device* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t udp_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct udp_devicedata* device_data = (struct udp_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->ip_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t udp_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);

    return 1;
}

void udp_read(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //  struct ip_devicedata* device_data = (struct ip_devicedata*)dev->device_data;
}
void udp_write(struct device* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    //   struct ip_devicedata* device_data = (struct ip_devicedata*)dev->device_data;
}

struct device* udp_attach(struct device* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->devicetype == IP);

    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &udp_init;
    deviceinstance->uninit = &udp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = UDP;
    devicemgr_set_device_description(deviceinstance, "User Datagram Protocol");
    /*
     * the device api
     */
    struct deviceapi_tcp* api = (struct deviceapi_tcp*)kmalloc(sizeof(struct deviceapi_tcp));
    memzero((uint8_t*)api, sizeof(struct deviceapi_tcp));
    api->read = &udp_read;
    api->write = &udp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct udp_devicedata* device_data = (struct udp_devicedata*)kmalloc(sizeof(struct udp_devicedata));
    device_data->ip_device = ip_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(ip_device);
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

void udp_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct udp_devicedata* device_data = (struct udp_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->ip_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}