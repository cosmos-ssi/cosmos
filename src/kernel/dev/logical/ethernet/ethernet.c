//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_ethernet.h>
#include <sys/objecttype/objecttype_nic.h>
#include <sys/string/mem.h>

struct ethernet_devicedata {
    struct object* nic_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t ethernet_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->nic_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t ethernet_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);

    return 1;
}

void ethernet_read(struct object* dev, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    struct objecttype_nic* nic_api = (struct objecttype_nic*)device_data->nic_device->api;
    nic_api->read(device_data->nic_device, (uint8_t*)eth, size);
}

void ethernet_write(struct object* dev, struct eth_hdr* eth, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    ASSERT_NOT_NULL(eth);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    struct objecttype_nic* nic_api = (struct objecttype_nic*)device_data->nic_device->api;
    nic_api->write(device_data->nic_device, (uint8_t*)eth, size);
}

struct object* ethernet_attach(struct object* nic_device) {
    ASSERT_NOT_NULL(nic_device);
    ASSERT((nic_device->devicetype == NIC) || (nic_device->devicetype == VNIC));
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_device();
    deviceinstance->init = &ethernet_init;
    deviceinstance->uninit = &ethernet_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = ETHERNET;
    objectmgr_set_device_description(deviceinstance, "Ethernet");
    /*
     * the device api
     */
    struct objecttype_ethernet* api = (struct objecttype_ethernet*)kmalloc(sizeof(struct objecttype_ethernet));
    memzero((uint8_t*)api, sizeof(struct objecttype_ethernet));
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
    if (0 != objectmgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_device_refcount(nic_device);
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

void ethernet_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct ethernet_devicedata* device_data = (struct ethernet_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_device_refcount(device_data->nic_device);
    /*
    * detach
    */
    objectmgr_detach_device(dev);
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
