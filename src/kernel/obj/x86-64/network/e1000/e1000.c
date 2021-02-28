//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/network/e1000/e1000.h>
#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectinterface/objectinterface_nic.h>

void e1000_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t e1000_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", obj->description, obj->pci->irq, obj->pci->vendor_id,
            obj->pci->device_id, obj->name);
    interrupt_router_register_interrupt_handler(obj->pci->irq, &e1000_irq_handler);
    return 1;
}

void e1000_ethernet_read(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);

    PANIC("Ethernet read not implemented yet");
}
void e1000_ethernet_write(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);

    PANIC("Ethernet write not implemented yet");
}

void e1000_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &e1000_init;
    objectinstance->pci = dev;
    objectinstance->objectype = NIC;
    objectmgr_set_object_description(objectinstance, "E1000 NIC");
    /*
     * the device api
     */
    struct objectinterface_nic* api = (struct objectinterface_nic*)kmalloc(sizeof(struct objectinterface_nic));
    api->write = &e1000_ethernet_read;
    api->read = &e1000_ethernet_write;
    objectinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 */
void e1000_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, 0x8086, 0x100E, &e1000_search_cb);
}
