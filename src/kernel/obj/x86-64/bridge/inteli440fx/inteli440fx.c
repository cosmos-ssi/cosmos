//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bridge/inteli440fx/inteli440fx.h>
#include <obj/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

struct intel440fx_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t i440fx_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct intel440fx_deviceddata* object_data = (struct intel440fx_deviceddata*)obj->object_data;
    object_data->base = pci_calcbar(obj->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);
    return 1;
}

void i440fx_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &i440fx_init;
    objectinstance->pci = dev;
    objectinstance->objectype = BRIDGE;
    objectmgr_set_object_description(objectinstance, "Intel i440FX PCI Bridge");
    /*
     * device data
     */
    struct intel440fx_deviceddata* object_data =
        (struct intel440fx_deviceddata*)kmalloc(sizeof(struct intel440fx_deviceddata));
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all bridge devices and register them
 */
void i440fx_bridge_register() {
    pci_objectmgr_search_objectype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_HOST, &i440fx_search_cb);
}
