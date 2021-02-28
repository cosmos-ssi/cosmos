//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_pit.h>
#include <sys/objectinterface/objectinterface_tick.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/string/mem.h>
#include <types.h>

struct tick_objectdata {
    struct object* pit_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tick_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->pit_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tick_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

uint64_t tick_read(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)obj->object_data;

    struct objectinterface_pit* api = (struct objectinterface_pit*)object_data->pit_device->api;
    return (*api->tickcount)(object_data->pit_device);
}

struct object* tick_attach(struct object* pit_device) {
    ASSERT_NOT_NULL(pit_device);
    ASSERT(pit_device->objectype == PIT);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &tick_init;
    objectinstance->uninit = &tick_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = TICK;
    objectmgr_set_object_description(objectinstance, "Tick Count");
    /*
     * the device api
     */
    struct objectinterface_tick* api = (struct objectinterface_tick*)kmalloc(sizeof(struct objectinterface_tick));
    memzero((uint8_t*)api, sizeof(struct objectinterface_tick));
    api->read = &tick_read;
    objectinstance->api = api;
    /*
     * device data
     */
    struct tick_objectdata* object_data = (struct tick_objectdata*)kmalloc(sizeof(struct tick_objectdata));
    object_data->pit_device = pit_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(pit_device);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void tick_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->pit_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}