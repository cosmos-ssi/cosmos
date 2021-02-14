//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/console/vga_console.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_console.h>
#include <sys/deviceapi/deviceapi_vga.h>
#include <sys/devicemgr/device.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <types.h>

#define CONSOLE_TAB_WIDTH 5

struct vga_console_devicedata {
    struct device* vga_device;
    uint16_t vga_console_x_width;
    uint16_t vga_console_y_height;
    uint8_t vga_console_xpos;
    uint8_t vga_console_ypos;
};

/*
 * perform device instance specific init here
 */
uint8_t vga_console_dev_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vga_console_devicedata* device_data = (struct vga_console_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->vga_device->name, dev->name);

    device_data->vga_console_xpos = 0;
    device_data->vga_console_ypos = 0;

    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)device_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    (*vga_api->query_resolution)(device_data->vga_device, &(device_data->vga_console_x_width),
                                 &(device_data->vga_console_y_height));
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t vga_console_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    struct vga_console_devicedata* device_data = (struct vga_console_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->vga_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

uint8_t vga_console_dev_setpos(struct device* dev, uint8_t x, uint8_t y) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vga_console_devicedata* device_data = (struct vga_console_devicedata*)dev->device_data;
    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)device_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    // error if position is out of range
    if ((x >= device_data->vga_console_x_width) || (y >= device_data->vga_console_y_height)) {
        return 0;
    }

    device_data->vga_console_xpos = x;
    device_data->vga_console_ypos = y;

    return 1;
}

void vga_console_dev_write(struct device* dev, const char* c) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vga_console_devicedata* device_data = (struct vga_console_devicedata*)dev->device_data;
    struct deviceapi_vga* vga_api = (struct deviceapi_vga*)device_data->vga_device->api;
    ASSERT_NOT_NULL(vga_api);

    uint64_t i =
        0;  // just in case...it's on the stack anyway, so it's a single-digit # of extra bytes that's gone once we return
    char s[2];

    s[1] = '\0';

    while (c[i]) {
        if (c[i] == '\n') {
            device_data->vga_console_xpos = 0;
            device_data->vga_console_ypos++;
            i++;

            continue;
        } else if (c[i] == '\t') {
            device_data->vga_console_xpos += (CONSOLE_TAB_WIDTH - (device_data->vga_console_xpos % CONSOLE_TAB_WIDTH));

            if (device_data->vga_console_xpos >= device_data->vga_console_x_width) {
                device_data->vga_console_xpos = 0;
                device_data->vga_console_ypos++;
            }

            i++;

            continue;
        }

        if ((device_data->vga_console_xpos >=
             device_data
                 ->vga_console_x_width)) {  // width is 1-based, pos is 0-based, so if pos = width then we're past the last column
            device_data->vga_console_xpos = 0;
            device_data->vga_console_ypos++;
        }

        if (device_data->vga_console_ypos >= device_data->vga_console_y_height) {
            // video_scroll_text();
            (*vga_api->scroll_text)(device_data->vga_device);

            device_data->vga_console_ypos =
                (device_data->vga_console_y_height - 1);  // again, ypos is a 0-based index, while height is a quantity
        }

        s[0] = c[i];

        //  video_write_text(s, device_data->vga_console_ypos, device_data->vga_console_xpos, NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);
        (*vga_api->write_text)(device_data->vga_device, s, device_data->vga_console_ypos, device_data->vga_console_xpos,
                               NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);

        device_data->vga_console_xpos++;
        i++;
    }
}

struct device* vga_console_attach(struct device* vga_device) {
    ASSERT_NOT_NULL(vga_device);
    ASSERT(vga_device->devicetype == VGA);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &vga_console_dev_init;
    deviceinstance->uninit = &vga_console_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = CONSOLE;
    devicemgr_set_device_description(deviceinstance, "VGA Console");
    /*
     * the device api
     */
    struct deviceapi_console* api = (struct deviceapi_console*)kmalloc(sizeof(struct deviceapi_console));
    memzero((uint8_t*)api, sizeof(struct deviceapi_console));
    api->setpos = &vga_console_dev_setpos;
    api->write = &vga_console_dev_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct vga_console_devicedata* device_data =
        (struct vga_console_devicedata*)kmalloc(sizeof(struct vga_console_devicedata));
    device_data->vga_device = vga_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(vga_device);
        /*
        * return device
        */
        return deviceinstance;
    } else {
        kfree(device_data);
        kfree(api);
        kfree(deviceinstance);
        return 0;
    }
}

void vga_console_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct vga_console_devicedata* device_data = (struct vga_console_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->vga_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}