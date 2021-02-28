//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/console/vga_console.h>
#include <obj/x86-64/serial/serial.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_console.h>
#include <sys/objecttype/objecttype_vga.h>
#include <sys/string/mem.h>

#define CONSOLE_TAB_WIDTH 5

struct vga_console_objectdata {
    struct object* vga_device;
    uint16_t vga_console_x_width;
    uint16_t vga_console_y_height;
    uint8_t vga_console_xpos;
    uint8_t vga_console_ypos;
};

/*
 * perform device instance specific init here
 */
uint8_t vga_console_dev_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vga_console_objectdata* object_data = (struct vga_console_objectdata*)obj->object_data;
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->vga_device->name, obj->name);

    object_data->vga_console_xpos = 0;
    object_data->vga_console_ypos = 0;

    struct objecttype_vga* vga_api = (struct objecttype_vga*)object_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    (*vga_api->query_resolution)(object_data->vga_device, &(object_data->vga_console_x_width),
                                 &(object_data->vga_console_y_height));
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t vga_console_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);

    struct vga_console_objectdata* object_data = (struct vga_console_objectdata*)obj->object_data;
    kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->vga_device->name, obj->name);
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

uint8_t vga_console_dev_setpos(struct object* obj, uint8_t x, uint8_t y) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vga_console_objectdata* object_data = (struct vga_console_objectdata*)obj->object_data;
    struct objecttype_vga* vga_api = (struct objecttype_vga*)object_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    // error if position is out of range
    if ((x >= object_data->vga_console_x_width) || (y >= object_data->vga_console_y_height)) {
        return 0;
    }

    object_data->vga_console_xpos = x;
    object_data->vga_console_ypos = y;

    return 1;
}

void vga_console_dev_write(struct object* obj, const char* c) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vga_console_objectdata* object_data = (struct vga_console_objectdata*)obj->object_data;
    struct objecttype_vga* vga_api = (struct objecttype_vga*)object_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    uint64_t i =
        0;  // just in case...it's on the stack anyway, so it's a single-digit # of extra bytes that's gone once we return
    char s[2];

    s[1] = '\0';

    while (c[i]) {
        if (c[i] == '\n') {
            object_data->vga_console_xpos = 0;
            object_data->vga_console_ypos++;
            i++;

            continue;
        } else if (c[i] == '\t') {
            object_data->vga_console_xpos += (CONSOLE_TAB_WIDTH - (object_data->vga_console_xpos % CONSOLE_TAB_WIDTH));

            if (object_data->vga_console_xpos >= object_data->vga_console_x_width) {
                object_data->vga_console_xpos = 0;
                object_data->vga_console_ypos++;
            }

            i++;

            continue;
        }

        if ((object_data->vga_console_xpos >=
             object_data
                 ->vga_console_x_width)) {  // width is 1-based, pos is 0-based, so if pos = width then we're past the last column
            object_data->vga_console_xpos = 0;
            object_data->vga_console_ypos++;
        }

        if (object_data->vga_console_ypos >= object_data->vga_console_y_height) {
            // video_scroll_text();
            (*vga_api->scroll_text)(object_data->vga_device);

            object_data->vga_console_ypos =
                (object_data->vga_console_y_height - 1);  // again, ypos is a 0-based index, while height is a quantity
        }

        s[0] = c[i];

        //  video_write_text(s, object_data->vga_console_ypos, object_data->vga_console_xpos, NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);
        (*vga_api->write_text)(object_data->vga_device, s, object_data->vga_console_ypos, object_data->vga_console_xpos,
                               NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);

        object_data->vga_console_xpos++;
        i++;
    }
}

struct object* vga_console_attach(struct object* vga_device) {
    ASSERT_NOT_NULL(vga_device);
    ASSERT(vga_device->objectype == VGA);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &vga_console_dev_init;
    objectinstance->uninit = &vga_console_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = CONSOLE;
    objectmgr_set_object_description(objectinstance, "VGA Console");
    /*
     * the device api
     */
    struct objecttype_console* api = (struct objecttype_console*)kmalloc(sizeof(struct objecttype_console));
    memzero((uint8_t*)api, sizeof(struct objecttype_console));
    api->setpos = &vga_console_dev_setpos;
    api->write = &vga_console_dev_write;
    objectinstance->api = api;
    /*
     * device data
     */
    struct vga_console_objectdata* object_data =
        (struct vga_console_objectdata*)kmalloc(sizeof(struct vga_console_objectdata));
    object_data->vga_device = vga_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(vga_device);
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

void vga_console_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct vga_console_objectdata* object_data = (struct vga_console_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->vga_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}