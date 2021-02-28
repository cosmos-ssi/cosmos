//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_ethernet.h>
#include <sys/deviceapi/deviceapi_nic.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/mem.h>

struct ethernet_devicedata {
    struct device* nic_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ethernet_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->nic_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ethernet_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);

    return 1;
}

void ethernet_read(struct device* dev, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    struct deviceapi_nic* nic_api = (struct deviceapi_nic*)device_data->nic_device->api;
    nic_api->read(device_data->nic_device, (uint8_t*)eth, size);
}

void ethernet_write(struct device* dev, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    struct deviceapi_nic* nic_api = (struct deviceapi_nic*)device_data->nic_device->api;
    nic_api->write(device_data->nic_device, (uint8_t*)eth, size);
}

struct device* ethernet_attach(struct device* nic_device) {
    ASSERT_NOT_NULL(nic_device);
    ASSERT((nic_device->devicetype == NIC) || (nic_device->devicetype == VNIC));
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &ethernet_init;
    deviceinstance->uninit = &ethernet_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ETHERNET;
    devicemgr_set_device_description(deviceinstance, "Ethernet");
    /*
     * the device api
     */
    struct deviceapi_ethernet* api = (struct deviceapi_ethernet*)kmalloc(sizeof(struct deviceapi_ethernet));
    memzero((uint8_t*)api, sizeof(struct deviceapi_ethernet));
    api->read = &ethernet_read;
    api->write = &ethernet_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)kmalloc(sizeof(struct ethernet_devicedata));
    device_data->nic_device = nic_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(nic_device);
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

void ethernet_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->nic_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}

void ethernet_init_eth(struct eth_hdr* eth, uint8_t* hw_source, uint8_t* hw_dest, uint8_t* data,
                       uint16_t payload_size) {
    ASSERT_NOT_NULL(eth);
    ASSERT_NOT_NULL(hw_source);
    ASSERT_NOT_NULL(hw_dest);
    ASSERT_NOT_NULL(data);
    ASSERT(payload_size <= 1500);
    memzero((uint8_t*)eth, sizeof(struct eth_hdr));
    memcpy((uint8_t*)&(eth->dest_hw), hw_dest, 6);
    memcpy((uint8_t*)&(eth->source_hw), hw_source, 6);
    eth->data = data;
    eth->type = payload_size;
}
