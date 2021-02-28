//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/objecttype/objecttype_tcp.h>
#include <sys/string/mem.h>
#include <types.h>

struct tcp_objectdata {
    struct object* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tcp_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->ip_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tcp_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    kfree(dev->object_data);

    return 1;
}

void tcp_read(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //  struct ip_objectdata* object_data = (struct ip_objectdata*)dev->object_data;
}
void tcp_write(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //   struct ip_objectdata* object_data = (struct ip_objectdata*)dev->object_data;
}

struct object* tcp_attach(struct object* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->devicetype == IP);

    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &tcp_init;
    deviceinstance->uninit = &tcp_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = TCP;
    objectmgr_set_object_description(deviceinstance, "Transmission Control Protocol");
    /*
     * the device api
     */
    struct objecttype_tcp* api = (struct objecttype_tcp*)kmalloc(sizeof(struct objecttype_tcp));
    memzero((uint8_t*)api, sizeof(struct objecttype_tcp));
    api->read = &tcp_read;
    api->write = &tcp_write;

    deviceinstance->api = api;
    /*
     * device data
     */
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)kmalloc(sizeof(struct tcp_objectdata));
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

void tcp_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ip_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}