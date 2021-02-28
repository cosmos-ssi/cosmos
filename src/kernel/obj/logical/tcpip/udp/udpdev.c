//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_tcp.h>
#include <sys/string/mem.h>

struct udp_objectdata {
    struct object* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t udp_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct udp_objectdata* object_data = (struct udp_objectdata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->ip_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t udp_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);

    return 1;
}

void udp_read(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //  struct ip_objectdata* object_data = (struct ip_objectdata*)dev->object_data;
}
void udp_write(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //   struct ip_objectdata* object_data = (struct ip_objectdata*)dev->object_data;
}

struct object* udp_attach(struct object* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->devicetype == IP);

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &udp_init;
    deviceinstance->uninit = &udp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = UDP;
    objectmgr_set_object_description(deviceinstance, "User Datagram Protocol");
    /*
     * the device api
     */
    struct objecttype_tcp* api = (struct objecttype_tcp*)kmalloc(sizeof(struct objecttype_tcp));
    memzero((uint8_t*)api, sizeof(struct objecttype_tcp));
    api->read = &udp_read;
    api->write = &udp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct udp_objectdata* object_data = (struct udp_objectdata*)kmalloc(sizeof(struct udp_objectdata));
    object_data->ip_device = ip_device;
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(ip_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(object_data);
        kfree(deviceinstance);
        return 0;
    }
}

void udp_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct udp_objectdata* object_data = (struct udp_objectdata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ip_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}