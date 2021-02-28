//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_pit.h>
#include <sys/objecttype/objecttype_tick.h>
#include <sys/string/mem.h>

struct tick_objectdata {
    struct object* pit_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t tick_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)dev->object_data;
    kprintf("Init %s on %s (%s)\n", dev->description, object_data->pit_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t tick_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);

    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

uint64_t tick_read(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)dev->object_data;

    struct objecttype_pit* api = (struct objecttype_pit*)object_data->pit_device->api;
    return (*api->tickcount)(object_data->pit_device);
}

struct object* tick_attach(struct object* pit_device) {
    ASSERT_NOT_NULL(pit_device);
    ASSERT(pit_device->devicetype == PIT);
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &tick_init;
    deviceinstance->uninit = &tick_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = TICK;
    objectmgr_set_object_description(deviceinstance, "Tick Count");
    /*
     * the device api
     */
    struct objecttype_tick* api = (struct objecttype_tick*)kmalloc(sizeof(struct objecttype_tick));
    memzero((uint8_t*)api, sizeof(struct objecttype_tick));
    api->read = &tick_read;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct tick_objectdata* object_data = (struct tick_objectdata*)kmalloc(sizeof(struct tick_objectdata));
    object_data->pit_device = pit_device;
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(pit_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void tick_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct tick_objectdata* object_data = (struct tick_objectdata*)dev->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->pit_device);
    /*
    * detach
    */
    objectmgr_detach_object(dev);
}