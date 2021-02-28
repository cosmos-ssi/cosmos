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
#include <sys/obj/objectinterface/objectinterface_null.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * perform device instance specific init here
 */
uint8_t null_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t null_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

uint8_t null_read(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    return 0;
}

struct object* null_attach() {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &null_init;
    objectinstance->uninit = &null_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_NULL0;
    objectmgr_set_object_description(objectinstance, "null");
    /*
     * the device api
     */
    struct objectinterface_null* api = (struct objectinterface_null*)kmalloc(sizeof(struct objectinterface_null));
    memzero((uint8_t*)api, sizeof(struct objectinterface_null));
    api->read = &null_read;
    objectinstance->api = api;
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

void null_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    objectmgr_detach_object(obj);
}