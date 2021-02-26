//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/kernelmap/kernelmap.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/deviceapi/deviceapi_kernelmap.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>

// defined in cosmos.ld
extern uint64_t _text_start;
extern uint64_t _text_end;
extern uint64_t _data_start;
extern uint64_t _data_end;
extern uint64_t _bss_start;
extern uint64_t _bss_end;
extern uint64_t _debug_start;
extern uint64_t _debug_end;

uint64_t text_start = (uint64_t)&_text_start;
uint64_t text_end = (uint64_t)&text_end;
uint64_t data_start = (uint64_t)&data_start;
uint64_t data_end = (uint64_t)&data_end;
uint64_t bss_start = (uint64_t)&bss_start;
uint64_t bss_end = (uint64_t)&bss_end;
uint64_t debug_start = (uint64_t)&debug_start;
uint64_t debug_end = (uint64_t)&debug_end;

/*
 * perform device instance specific init here
 */
uint8_t kernelmap_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s (%s)\n", dev->description, dev->name);
    return 1;
}

void kernelmap_read(struct device* dev, struct kernelmap* km) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(km);
    km->text_start = text_start;
    km->text_end = text_end;
    km->data_start = data_start;
    km->data_end = data_end;
    km->bss_start = bss_start;
    km->bss_end = bss_end;
    km->debug_start = debug_start;
    km->debug_end = debug_end;
}

void kernelmap_devicemgr_register_devices() {
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    devicemgr_set_device_description(deviceinstance, "Kernel map");
    deviceinstance->devicetype = KERNELMAP;
    deviceinstance->init = &kernelmap_device_init;
    /*
     * api
     */
    struct deviceapi_kernelmap* api = (struct deviceapi_kernelmap*)kmalloc(sizeof(struct deviceapi_kernelmap));
    api->read = &kernelmap_read;
    deviceinstance->api = api;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}