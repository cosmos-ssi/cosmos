//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objectinterface/objectinterface_tcp.h>
#include <sys/string/mem.h>

struct tcp_objectdata {
    struct object* ip_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tcp_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->ip_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tcp_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);

    return 1;
}

void tcp_read(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}
void tcp_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct ip_objectdata* object_data = (struct ip_objectdata*)obj->object_data;
}

struct object* tcp_attach(struct object* ip_device) {
    ASSERT_NOT_NULL(ip_device);
    ASSERT(ip_device->objectype == IP);

    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &tcp_init;
    objectinstance->uninit = &tcp_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = TCP;
    objectmgr_set_object_description(objectinstance, "Transmission Control Protocol");
    /*
     * the device api
     */
    struct objectinterface_tcp* api = (struct objectinterface_tcp*)kmalloc(sizeof(struct objectinterface_tcp));
    memzero((uint8_t*)api, sizeof(struct objectinterface_tcp));
    api->read = &tcp_read;
    api->write = &tcp_write;

    objectinstance->api = api;
    /*
     * device data
     */
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)kmalloc(sizeof(struct tcp_objectdata));
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

void tcp_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tcp_objectdata* object_data = (struct tcp_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->ip_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}