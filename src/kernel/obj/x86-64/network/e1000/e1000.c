//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/network/e1000/e1000.h>
#include <obj/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_nic.h>

void e1000_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t e1000_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id,
            dev->pci->device_id, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &e1000_irq_handler);
    return 1;
}

void e1000_ethernet_read(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    PANIC("Ethernet read not implemented yet");
}
void e1000_ethernet_write(struct object* dev, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(data);

    PANIC("Ethernet write not implemented yet");
}

void e1000_search_cb(struct pci_device* dev) {
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &e1000_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = NIC;
    objectmgr_set_object_description(deviceinstance, "E1000 NIC");
    /*
     * the device api
     */
    struct objecttype_nic* api = (struct objecttype_nic*)kmalloc(sizeof(struct objecttype_nic));
    api->write = &e1000_ethernet_read;
    api->read = &e1000_ethernet_write;
    deviceinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(deviceinstance);
}

/**
 */
void e1000_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, 0x8086, 0x100E, &e1000_search_cb);
}
