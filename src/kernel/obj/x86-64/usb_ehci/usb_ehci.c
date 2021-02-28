//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <obj/x86-64/usb_ehci/usb_ehci.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <types.h>

struct usbcontroller_objectdata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t usb_ehci_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct usbcontroller_objectdata* object_data = (struct usbcontroller_objectdata*)obj->object_data;
    object_data->base = pci_calcbar(obj->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);
    return 1;
}

void usb_ehci_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &usb_ehci_obj_init;
    objectinstance->pci = dev;
    objectinstance->objectype = USB;
    objectmgr_set_object_description(objectinstance, "Intel 82801 USB EHCI Controller");
    /*
     * the object_data
     */
    struct usbcontroller_objectdata* object_data =
        (struct usbcontroller_objectdata*)kmalloc(sizeof(struct usbcontroller_objectdata));
    object_data->base = 0;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

/**
 * find all USB devices and register them
 */
void usb_ehci_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_SERIAL, PCI_SERIAL_SUBCLASS_USB, 0x8086, 0x24CD, &usb_ehci_search_cb);
}
