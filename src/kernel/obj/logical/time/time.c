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
#include <sys/obj/objectinterface/objectinterface_rtc.h>
#include <sys/obj/objectinterface/objectinterface_time.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

struct time_objectdata {
    struct object* rtc_device;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t time_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct time_objectdata* object_data = (struct time_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->rtc_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t time_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

time_time_t time_time(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct time_objectdata* object_data = (struct time_objectdata*)obj->object_data;

    struct objectinterface_rtc* api = (struct objectinterface_rtc*)object_data->rtc_device->api;
    rtc_time_t t = (*api->rtc_time)(object_data->rtc_device);
    time_time_t t2;
    t2.century = t.century;
    t2.hour = t.hour;
    t2.minute = t.minute;
    t2.second = t.second;
    t2.month = t.month;
    t2.monthday = t.monthday;
    t2.weekday = t.weekday;
    t2.year = t.year;
    return t2;
}

struct object* time_attach(struct object* rtc_device) {
    ASSERT_NOT_NULL(rtc_device);
    ASSERT(rtc_device->objectype == OBJECT_TYPE_RTC);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &time_init;
    objectinstance->uninit = &time_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_TIME;
    objectmgr_set_object_description(objectinstance, "Time");
    /*
     * the device api
     */
    struct objectinterface_time* api = (struct objectinterface_time*)kmalloc(sizeof(struct objectinterface_time));
    memzero((uint8_t*)api, sizeof(struct objectinterface_time));
    api->time = &time_time;
    objectinstance->api = api;
    /*
     * device data
     */
    struct time_objectdata* object_data = (struct time_objectdata*)kmalloc(sizeof(struct time_objectdata));
    object_data->rtc_device = rtc_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(rtc_device);
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

void time_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct time_objectdata* object_data = (struct time_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->rtc_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}