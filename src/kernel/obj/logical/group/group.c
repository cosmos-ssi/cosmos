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
#include <sys/obj/objectinterface/objectinterface_group.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#include <types.h>

#define GROUP_MAX_NAME 32

struct group_objectdata {
    uint8_t name[GROUP_MAX_NAME];
};

/*
 * perform device instance specific init here
 */
uint8_t group_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct group_objectdata* object_data = (struct group_objectdata*)obj->object_data;
    kprintf("Init %s (%s)\n", obj->description, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t group_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

struct object* group_attach(uint8_t* name) {
    ASSERT_NOT_NULL(name);
    ASSERT(strlen(name) < GROUP_MAX_NAME);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &group_init;
    objectinstance->uninit = &group_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_GROUP;
    objectmgr_set_object_description(objectinstance, "Group");
    /*
     * the device api
     */
    struct objectinterface_group* api = (struct objectinterface_group*)kmalloc(sizeof(struct objectinterface_group));
    memzero((uint8_t*)api, sizeof(struct objectinterface_group));
    //    api->read = &group_read;
    objectinstance->api = api;
    /*
     * device data
     */
    struct group_objectdata* object_data = (struct group_objectdata*)kmalloc(sizeof(struct group_objectdata));
    strncpy(object_data->name, name, GROUP_MAX_NAME);
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

void group_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct group_objectdata* object_data = (struct group_objectdata*)obj->object_data;
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}
