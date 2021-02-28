/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/pci/pci.h>
#include <obj/x86-64/sdhci/sdhci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>

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
uint8_t sdhci_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct sdhci_objectdata* object_data = (struct sdhci_objectdata*)obj->object_data;
    object_data->base = (uint64_t)CONV_PHYS_ADDR(pci_calcbar(obj->pci));
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);
    interrupt_router_register_interrupt_handler(obj->pci->irq, &sdhci_irq_handler);
    return 1;
}

/*
 * perform device instance specific uninit here
 */
uint8_t sdhci_obj_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Uninit %s (%s)\n", obj->description, obj->name);
    return 1;
}

void sdhci_pci_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectmgr_set_object_description(objectinstance, "SDHCI Controller");
    objectinstance->objectype = SDHCI;
    objectinstance->pci = dev;
    objectinstance->init = &sdhci_obj_init;
    objectinstance->uninit = sdhci_obj_uninit;
    /*
     * device api
     */
    //   struct objectinterface_block* api = (struct objectinterface_block*)kmalloc(sizeof(struct objectinterface_block));
    //   memzero((uint8_t*)api, sizeof(struct objectinterface_block));
    //   objectinstance->api = api;
    /*
     * the object_data
     */
    struct sdhci_objectdata* object_data = (struct sdhci_objectdata*)kmalloc(sizeof(struct sdhci_objectdata));
    object_data->base = 0;
    objectinstance->object_data = object_data;
    /**
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all NE2000 devices and register them
 */
void sdhci_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_BASE_PERI, 0x05, 0x1B36, 0x07, &sdhci_pci_search_cb);
}
