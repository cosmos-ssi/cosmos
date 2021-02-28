//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/console/serial_console.h>
#include <dev/x86-64/serial/serial.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/objecttype/objecttype_console.h>
#include <sys/objecttype/objecttype_serial.h>
#include <sys/string/mem.h>

struct serial_console_devicedata {
    struct device* serial_device;
};

/*
 * perform device instance specific init here
 */
uint8_t serial_console_dev_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct serial_console_devicedata* device_data = (struct serial_console_devicedata*)dev->device_data;
    kprintf("Init %s on %s (%s)\n", dev->description, device_data->serial_device->name, dev->name);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t serial_console_uninit(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);

    struct serial_console_devicedata* device_data = (struct serial_console_devicedata*)dev->device_data;
    kprintf("Uninit %s on %s (%s)\n", dev->description, device_data->serial_device->name, dev->name);
    kfree(dev->api);
    kfree(dev->device_data);
    return 1;
}

uint8_t serial_console_setpos(struct device* dev, uint8_t x, uint8_t y) {
    // do nothing
    return 0;
}

void serial_console_dev_write(struct device* dev, const char* s) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct serial_console_devicedata* device_data = (struct serial_console_devicedata*)dev->device_data;

    struct objecttype_serial* serial_api = (struct objecttype_serial*)device_data->serial_device->api;
    (*serial_api->write)(device_data->serial_device, s);
}

struct device* serial_console_attach(struct device* serial_device) {
    ASSERT_NOT_NULL(serial_device);
    ASSERT(serial_device->devicetype == SERIAL);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &serial_console_dev_init;
    deviceinstance->uninit = &serial_console_uninit;
    deviceinstance->pci = 0;
    deviceinstance->devicetype = CONSOLE;
    devicemgr_set_device_description(deviceinstance, "Serial Console");
    /*
     * the device api
     */
    struct objecttype_console* api = (struct objecttype_console*)kmalloc(sizeof(struct objecttype_console));
    memzero((uint8_t*)api, sizeof(struct objecttype_console));
    api->setpos = &serial_console_setpos;
    api->write = &serial_console_dev_write;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct serial_console_devicedata* device_data =
        (struct serial_console_devicedata*)kmalloc(sizeof(struct serial_console_devicedata));
    device_data->serial_device = serial_device;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    if (0 != devicemgr_attach_device(deviceinstance)) {
        /*
        * increase ref count of underlying device
        */
        devicemgr_increment_device_refcount(serial_device);
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

void serial_console_detach(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->device_data);
    struct serial_console_devicedata* device_data = (struct serial_console_devicedata*)dev->device_data;
    /*
    * decrease ref count of underlying device
    */
    devicemgr_decrement_device_refcount(device_data->serial_device);
    /*
    * detach
    */
    devicemgr_detach_device(dev);
}