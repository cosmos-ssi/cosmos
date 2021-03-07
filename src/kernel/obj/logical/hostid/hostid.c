//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/hostid/hostid.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_hostid.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

struct hostid_objectdata {
    uint64_t hostid;
};

/*
 * perform device instance specific init here
 */
uint8_t hostid_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct hostid_objectdata* object_data = (struct hostid_objectdata*)obj->object_data;
    kprintf("Init %s (%s) with id %#llX\n", obj->description, obj->name, object_data->hostid);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t hostid_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

uint64_t hostid_read(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct hostid_objectdata* object_data = (struct hostid_objectdata*)obj->object_data;
    return object_data->hostid;
}

struct object* hostid_attach() {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &hostid_init;
    objectinstance->uninit = &hostid_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_HOSTID;
    objectmgr_set_object_description(objectinstance, "Host ID");
    /*
     * the device api
     */
    struct objectinterface_hostid* api = (struct objectinterface_hostid*)kmalloc(sizeof(struct objectinterface_hostid));
    memzero((uint8_t*)api, sizeof(struct objectinterface_hostid));
    api->read = &hostid_read;
    objectinstance->api = api;
    /*
     * device data
     */
    struct hostid_objectdata* object_data = (struct hostid_objectdata*)kmalloc(sizeof(struct hostid_objectdata));
    object_data->hostid = 12;
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

void hostid_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    //   ASSERT_NOT_NULL(obj->object_data);
    //   struct hostid_objectdata* object_data = (struct hostid_objectdata*)obj->object_data;
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}