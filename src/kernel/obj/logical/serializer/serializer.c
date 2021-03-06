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
#include <sys/obj/objectinterface/objectinterface_serializer.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

/*
 * perform device instance specific init here
 */
uint8_t serializer_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t serializer_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

void serializer_serialize(struct object* obj) {
    ASSERT_NOT_NULL(obj);
}
void serializer_deserialize(struct object* obj) {
    ASSERT_NOT_NULL(obj);
}

struct object* serializer_attach() {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &serializer_init;
    objectinstance->uninit = &serializer_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_SERIALIZER;
    objectmgr_set_object_description(objectinstance, "Serializer");
    /*
     * the device api
     */
    struct objectinterface_serializer* api =
        (struct objectinterface_serializer*)kmalloc(sizeof(struct objectinterface_serializer));
    memzero((uint8_t*)api, sizeof(struct objectinterface_serializer));
    api->serialize = &serializer_serialize;
    api->deserialize = &serializer_deserialize;
    objectinstance->api = api;
    /*
     * device data
     */
    //  struct serializer_objectdata* object_data =
    // (struct serializer_objectdata*)kmalloc(sizeof(struct serializer_objectdata));
    //  object_data->last = 7;
    // objectinstance->object_data = object_data;
    objectinstance->object_data = 0;
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

void serializer_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    objectmgr_detach_object(obj);
}