//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_rand.h>
#include <sys/string/mem.h>
#include <types.h>

uint64_t RAND_MAX = (1 << 31);

struct rand_objectdata {
    uint64_t last;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t rand_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t rand_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);

    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    kfree(dev->api);
    return 1;
}
/*
* https://rosettacode.org/wiki/Linear_congruential_generator#C
*/
uint64_t rand_read(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct rand_objectdata* object_data = (struct rand_objectdata*)dev->object_data;
    object_data->last = (object_data->last * 214013 + 2531011) & RAND_MAX >> 16;
    return object_data->last;
}

struct object* rand_attach() {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &rand_init;
    deviceinstance->uninit = &rand_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = RAND;
    objectmgr_set_object_description(deviceinstance, "LCG Random Number Generator");
    /*
     * the device api
     */
    struct objecttype_rand* api = (struct objecttype_rand*)kmalloc(sizeof(struct objecttype_rand));
    memzero((uint8_t*)api, sizeof(struct objecttype_rand));
    api->read = &rand_read;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct rand_objectdata* object_data = (struct rand_objectdata*)kmalloc(sizeof(struct rand_objectdata));
    object_data->last = 7;
    deviceinstance->object_data = object_data;
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

void rand_detach(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    objectmgr_detach_object(dev);
}