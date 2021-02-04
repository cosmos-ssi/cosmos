//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/x86-64/bridge/intelpciisa/intelpciisa.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>

struct intelisapca_deviceddata {
    uint64_t base;
} __attribute__((packed));

/*
 * perform device instance specific init here
 */
uint8_t pciisa_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct intelisapca_deviceddata* device_data = (struct intelisapca_deviceddata*)dev->device_data;
    device_data->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, device_data->base, dev->name);
    return 1;
}

void pciisa_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &pciisa_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BRIDGE;
    devicemgr_set_device_description(deviceinstance, "Intel PIIX3 PCI-to-ISA Bridge (Triton II)");
    /*
     * device data
     */
    struct intelisapca_deviceddata* device_data =
        (struct intelisapca_deviceddata*)kmalloc(sizeof(struct intelisapca_deviceddata));
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

/**
 * find all bridge devices and register them
 */
void pciisa_bridge_register() {
    pci_devicemgr_search_devicetype(PCI_CLASS_BRIDGE, PCI_BRIDGE_SUBCLASS_ISA, &pciisa_search_cb);
}
