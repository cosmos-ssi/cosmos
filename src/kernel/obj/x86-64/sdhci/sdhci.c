/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <obj/x86-64/sdhci/sdhci.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/x86-64/idt/irq.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

#define SDHCI_COMMAND 0x00
#define SDHCI_RESPONSE 0x10
#define SDHCI_BUFFER 0x20
#define SDHCI_HOST_CONTROL 0x24
#define SDHCI_INTERRUPT 0x30

// https://www.sdcard.org/downloads/pls/

struct sdhci_objectdata {
    uint64_t base;
} __attribute__((packed));

void sdhci_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("?");
}

/*
 * perform device instance specific init here
 */
uint8_t sdhci_obj_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    struct sdhci_objectdata* object_data = (struct sdhci_objectdata*)dev->object_data;
    object_data->base = (uint64_t)CONV_PHYS_ADDR(pci_calcbar(dev->pci));
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, object_data->base, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &sdhci_irq_handler);
    return 1;
}

/*
 * perform device instance specific uninit here
 */
uint8_t sdhci_obj_uninit(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Uninit %s (%s)\n", dev->description, dev->name);
    return 1;
}

void sdhci_pci_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    objectmgr_set_object_description(deviceinstance, "SDHCI Controller");
    deviceinstance->devicetype = SDHCI;
    deviceinstance->pci = dev;
    deviceinstance->init = &sdhci_obj_init;
    deviceinstance->uninit = sdhci_obj_uninit;
    /*
     * device api
     */
    //   struct objecttype_block* api = (struct objecttype_block*)kmalloc(sizeof(struct objecttype_block));
    //   memzero((uint8_t*)api, sizeof(struct objecttype_block));
    //   deviceinstance->api = api;
    /*
     * the object_data
     */
    struct sdhci_objectdata* object_data = (struct sdhci_objectdata*)kmalloc(sizeof(struct sdhci_objectdata));
    object_data->base = 0;
    deviceinstance->object_data = object_data;
    /**
     * register
     */
    objectmgr_register_object(deviceinstance);
}

/**
 * find all NE2000 devices and register them
 */
void sdhci_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_BASE_PERI, 0x05, 0x1B36, 0x07, &sdhci_pci_search_cb);
}
