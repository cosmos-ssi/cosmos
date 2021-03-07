//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/exit_command/exit_command.h>
#include <obj/logical/telnet/commands/show_object_types_command/show_object_types_command.h>
#include <obj/logical/telnet/commands/show_objects_command/show_objects_command.h>
#include <obj/logical/telnet/commands/show_voh_command/show_voh_command.h>
#include <obj/logical/telnet/commands/test_command/test_command.h>
#include <obj/logical/telnet/telnet.h>
#include <obj/logical/telnet/telnet_commandloop.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_telnet.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <types.h>

struct telnet_objectdata {
    struct object* serial_device;
    struct arraylist* commands;

} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t telnet_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct telnet_objectdata* object_data = (struct telnet_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->serial_device->name, obj->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t telnet_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    kfree(obj->api);
    return 1;
}

void telnet_start(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct telnet_objectdata* object_data = (struct telnet_objectdata*)obj->object_data;
    ASSERT_NOT_NULL(object_data->serial_device);
    telnet_command_loop(object_data->serial_device, object_data->commands);
}

void telnet_add_commands(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct telnet_objectdata* object_data = (struct telnet_objectdata*)obj->object_data;
    ASSERT_NOT_NULL(object_data->commands);
    arraylist_add(object_data->commands, show_voh_new());
    arraylist_add(object_data->commands, show_objects_new());
    arraylist_add(object_data->commands, show_object_types_new());
    arraylist_add(object_data->commands, exit_new());
    arraylist_add(object_data->commands, test_new());
}

struct object* telnet_attach(struct object* serial_device) {
    ASSERT_NOT_NULL(serial_device);
    ASSERT(serial_device->objectype == OBJECT_TYPE_SERIAL);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &telnet_init;
    objectinstance->uninit = &telnet_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_TELNET;
    objectmgr_set_object_description(objectinstance, "Kernel Telnet");
    /*
     * the device api
     */
    struct objectinterface_telnet* api = (struct objectinterface_telnet*)kmalloc(sizeof(struct objectinterface_telnet));
    memzero((uint8_t*)api, sizeof(struct objectinterface_telnet));
    api->start = &telnet_start;
    objectinstance->api = api;

    /*
     * device data
     */
    struct telnet_objectdata* object_data = (struct telnet_objectdata*)kmalloc(sizeof(struct telnet_objectdata));
    object_data->serial_device = serial_device;
    object_data->commands = arraylist_new();
    objectinstance->object_data = object_data;
    telnet_add_commands(objectinstance);

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
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void telnet_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct telnet_objectdata* object_data = (struct telnet_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->serial_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}