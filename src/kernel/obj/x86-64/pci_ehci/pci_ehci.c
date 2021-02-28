/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/logical/ehci/ehci.h>
#include <obj/x86-64/pci/pci.h>
#include <obj/x86-64/pci_ehci/pci_ehci.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

// https://wiki.osdev.org/Enhanced_Host_Controller_Interface

void pci_ehci_handle_irq(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t pci_ehci_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    kprintf("Init %s at IRQ %llu\n", obj->description, obj->pci->irq);
    interrupt_router_register_interrupt_handler(obj->pci->irq, &pci_ehci_handle_irq);
    return 1;
}

void pci_ehci_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &pci_ehci_obj_init;
    objectinstance->pci = dev;
    objectinstance->objectype = BRIDGE;
    objectmgr_set_object_description(objectinstance, "PCI EHCI Controller");
    /*
     * device data
     */
    //	struct intelisapca_deviceddata* object_data = (struct intelisapca_deviceddata*) kmalloc(sizeof(struct intelisapca_deviceddata));
    //	objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all bridge devices and register them
 */
void pci_ehci_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_BASE_PERI, 0x05, 0x1B36, 0x07, &pci_ehci_search_cb);
}
