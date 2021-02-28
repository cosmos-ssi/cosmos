//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bridge/intelpciisa/intelpciisa.h>
#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <types.h>

struct intelisapca_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t pciisa_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct intelisapca_deviceddata* object_data = (struct intelisapca_deviceddata*)obj->object_data;
    object_data->base = pci_calcbar(obj->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);
    return 1;
}

void pciisa_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &pciisa_init;
    objectinstance->pci = dev;
    objectinstance->objectype = BRIDGE;
    objectmgr_set_object_description(objectinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
    /*
     * device data
     */
    struct intelisapca_deviceddata* object_data =
        (struct intelisapca_deviceddata*)kmalloc(sizeof(struct intelisapca_deviceddata));
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all bridge devices and register them
 */
void pciisa_bridge_register() {
    pci_objectmgr_search_objectype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_ISA, &pciisa_search_cb);
}
