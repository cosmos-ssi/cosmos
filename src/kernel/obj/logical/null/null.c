//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_null.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * perform device instance specific init here
 */
uint8_t null_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t null_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}

uint8_t null_read(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    return 0;
}

struct object* null_attach() {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &null_init;
    deviceinstance->uninit = &null_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = NULL0;
    objectmgr_set_object_description(deviceinstance, "null");
    /*
     * the device api
     */
    struct objecttype_null* api = (struct objecttype_null*)kmalloc(sizeof(struct objecttype_null));
    memzero((uint8_t*)api, sizeof(struct objecttype_null));
    api->read = &null_read;
    deviceinstance->api = api;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(deviceinstance)) {
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

void null_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    objectmgr_detach_object(dev);
}