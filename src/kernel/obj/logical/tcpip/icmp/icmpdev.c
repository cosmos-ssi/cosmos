//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_ip.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/string/mem.h>
#include <types.h>

struct icmp_objectdata {
    struct object* ethernet_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t icmp_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct icmp_objectdata* object_data = (struct icmp_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->ethernet_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t icmp_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);

    return 1;
}

void icmp_read(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct icmp_objectdata* object_data = (struct icmp_objectdata*)obj->object_data;
}
void icmp_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct icmp_objectdata* object_data = (struct icmp_objectdata*)obj->object_data;
}

struct object* icmp_attach(struct object* ethernet_device) {
    ASSERT_NOT_NULL(ethernet_device);
    ASSERT(ethernet_device->objectype == ETHERNET);

    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &icmp_init;
    objectinstance->uninit = &icmp_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = ICMP;
    objectmgr_set_object_description(objectinstance, "Internet Control Message Protocol");
    /*
     * the device api
     */
    struct objectinterface_ip* api = (struct objectinterface_ip*)kmalloc(sizeof(struct objectinterface_ip));
    memzero((uint8_t*)api, sizeof(struct objectinterface_ip));
    api->read = &icmp_read;
    api->write = &icmp_write;

    objectinstance->api = api;
    /*
     * device data
     */
    struct icmp_objectdata* object_data = (struct icmp_objectdata*)kmalloc(sizeof(struct icmp_objectdata));
    object_data->ethernet_device = ethernet_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(ethernet_device);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(api);
        kfree(object_data);
        kfree(objectinstance);
        return 0;
    }
}

void icmp_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct icmp_objectdata* object_data = (struct icmp_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ethernet_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}