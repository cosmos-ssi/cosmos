//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/ehci/ehci.h>
#include <dev/x86-64/pci/pci.h>
#include <dev/x86-64/usb_ehci/usb_ehci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

struct usbcontroller_devicedata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t usb_ehci_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct usbcontroller_devicedata* device_data = (struct usbcontroller_devicedata*)dev->device_data;
    device_data->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, device_data->base, dev->name);
    return 1;
}

void usb_ehci_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &usb_ehci_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = USB;
    devicemgr_set_device_description(deviceinstance, "Intel 82801 USB EHCI Controller");
    /*
     * the device_data
     */
    struct usbcontroller_devicedata* device_data =
        (struct usbcontroller_devicedata*)kmalloc(sizeof(struct usbcontroller_devicedata));
    device_data->base = 0;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all USB devices and register them
 */
void usb_ehci_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_SERIAL, PCI_SERIAL_SUBCLASS_USB, 0x8086, 0x24CD, &usb_ehci_search_cb);
}
