//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bridge/intel82371ab/intel82371ab.h>
#include <obj/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

struct intel8237_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t i982371_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    struct intel8237_deviceddata* object_data = (struct intel8237_deviceddata*)dev->object_data;
    object_data->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, object_data->base, dev->name);
    return 1;
}

void i982371_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &i982371_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    objectmgr_set_object_description(deviceinstance, "Intel PIIX4/4E/4M Power Management Controller");
    /*
     * device data
     */
    struct intel8237_deviceddata* object_data =
        (struct intel8237_deviceddata*)kmalloc(sizeof(struct intel8237_deviceddata));
    deviceinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}

/**
 * find all bridge devices and register them
 */
void i982371_bridge_register() {
    pci_objectmgr_search_devicetype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_OTHER, &i982371_search_cb);
}
