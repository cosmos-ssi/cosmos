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
#include <sys/obj/objectinterface/objectinterface_rand.h>
#include <sys/obj/objectinterface/objectinterface_time.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

uint64_t RAND_MAX = (1 << 31);

struct rand_objectdata {
    uint64_t last;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t rand_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct rand_objectdata* object_data = (struct rand_objectdata*)obj->object_data;
    kprintf("Init %s (%s) with seed %#llX\n", obj->description, obj->name, object_data->last);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t rand_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

uint64_t rand_next(uint64_t x) {
    return (x * 214013 + 2531011) & RAND_MAX >> 16;
}

/*
* https://rosettacode.org/wiki/Linear_congruential_generator#C
*/
uint64_t rand_read(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct rand_objectdata* object_data = (struct rand_objectdata*)obj->object_data;
    object_data->last = rand_next(object_data->last);
    return object_data->last;
}

uint64_t rand_get_time_value(struct object* time) {
    struct objectinterface_time* api = (struct objectinterface_time*)time->api;
    struct time_time_t t = (*api->time)(time);
    //  kprintf("s %#llX, m %#llX, h %#llX, d %#llX\n", t.second, t.minute, t.hour, t.monthday);
    uint64_t x1 = t.second | (t.minute << 8) | (t.hour << 16) | (t.monthday << 24);
    return rand_next(x1);
}

struct object* rand_attach(struct object* time) {
    ASSERT_NOT_NULL(time);
    ASSERT(time->objectype = OBJECT_TYPE_TIME);

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &rand_init;
    objectinstance->uninit = &rand_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_RAND;
    objectmgr_set_object_description(objectinstance, "LCG Random Number Generator");
    /*
     * the device api
     */
    struct objectinterface_rand* api = (struct objectinterface_rand*)kmalloc(sizeof(struct objectinterface_rand));
    memzero((uint8_t*)api, sizeof(struct objectinterface_rand));
    api->read = &rand_read;
    objectinstance->api = api;
    /*
     * device data
     */
    struct rand_objectdata* object_data = (struct rand_objectdata*)kmalloc(sizeof(struct rand_objectdata));
    object_data->last = rand_get_time_value(time);
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
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

void rand_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    objectmgr_detach_object(obj);
}