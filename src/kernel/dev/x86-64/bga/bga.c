//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/bga/bga.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_bga.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

char* bga_graphic_mem_base = (char*)0xA0000;

////struct bga_devicedata {};

/*
 * perform device instance specific init here
 */
uint8_t bga_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    //   ASSERT_NOT_NULL(dev->device_data);
    //  struct bga_devicedata* device_data = (struct bga_devicedata*)dev->device_data;

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id,
            dev->pci->device_id, dev->name);
    return 1;
}

void bga_clear(struct device* dev) {
    ASSERT_NOT_NULL(dev);
}

void bga_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &bga_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BGA;
    devicemgr_set_device_description(deviceinstance, "QEMU/Bochs BGA Framebuffer");
    devicemgr_register_device(deviceinstance);
    /*
     * device api
     */
    struct deviceapi_bga* api = (struct deviceapi_bga*)kmalloc(sizeof(struct deviceapi_bga));
    api->clear = &bga_clear;
    deviceinstance->api = api;

    /*
     * device data
     */
    deviceinstance->device_data = 0;
    //   struct bga_devicedata* device_data = (struct bga_devicedata*)kmalloc(sizeof(struct bga_devicedata));
    //   deviceinstance->device_data = device_data;
}

/**
 * find all Display devices and register them
 */
void bga_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_DISPLAY, PCI_DISPLAY_SUBCLASS_VGA, &bga_search_cb);
}
