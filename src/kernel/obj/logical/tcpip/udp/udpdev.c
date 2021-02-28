//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_tcp.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

struct udp_objectdata {
    struct object* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t udp_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct udp_objectdata* object_data = (struct udp_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->ip_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t udp_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);

    return 1;
}

void udp_read(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}
void udp_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}

struct object* udp_attach(struct object* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->objectype == IP);

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &udp_init;
    objectinstance->uninit = &udp_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = UDP;
    objectmgr_set_object_description(objectinstance, "User Datagram Protocol");
    /*
     * the device api
     */
    struct objectinterface_tcp* api = (struct objectinterface_tcp*)kmalloc(sizeof(struct objectinterface_tcp));
    memzero((uint8_t*)api, sizeof(struct objectinterface_tcp));
    api->read = &udp_read;
    api->write = &udp_write;

    objectinstance->api = api;
    /*
     * device data
     */
    struct udp_objectdata* object_data = (struct udp_objectdata*)kmalloc(sizeof(struct udp_objectdata));
    object_data->ip_device = ip_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(ip_device);
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

void udp_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct udp_objectdata* object_data = (struct udp_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ip_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}