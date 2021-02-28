//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/console/serial_console.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectinterface/objectinterface_console.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <sys/string/mem.h>
#include <types.h>

struct serial_console_objectdata {
    struct object* serial_device;
};

/*
 * perform device instance specific init here
 */
uint8_t serial_console_dev_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct serial_console_objectdata* object_data = (struct serial_console_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->serial_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t serial_console_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    struct serial_console_objectdata* object_data = (struct serial_console_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->serial_device->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

uint8_t serial_console_setpos(struct object* obj, uint8_t x, uint8_t y) {
    // do nothing
    return 0;
}

void serial_console_dev_write(struct object* obj, const char* s) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct serial_console_objectdata* object_data = (struct serial_console_objectdata*)obj->object_data;

    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)object_data->serial_device->api;
    (*serial_api->write)(object_data->serial_device, s);
}

struct object* serial_console_attach(struct object* serial_device) {
    ASSERT_NOT_NULL(serial_device);
    ASSERT(serial_device->objectype == SERIAL);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &serial_console_dev_init;
    objectinstance->uninit = &serial_console_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = CONSOLE;
    objectmgr_set_object_description(objectinstance, "Serial Console");
    /*
     * the device api
     */
    struct objectinterface_console* api =
        (struct objectinterface_console*)kmalloc(sizeof(struct objectinterface_console));
    memzero((uint8_t*)api, sizeof(struct objectinterface_console));
    api->setpos = &serial_console_setpos;
    api->write = &serial_console_dev_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct serial_console_objectdata* object_data =
        (struct serial_console_objectdata*)kmalloc(sizeof(struct serial_console_objectdata));
    object_data->serial_device = serial_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(serial_device);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void serial_console_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct serial_console_objectdata* object_data = (struct serial_console_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->serial_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}