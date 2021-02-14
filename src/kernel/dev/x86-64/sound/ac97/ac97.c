/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/sound/ac97/ac97.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_dsp.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <dev/x86-64/pci/devicetree.h>
#include <dev/x86-64/pci/pci_device.h>
#include <sys/devicemgr/device.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/x86-64/idt/irq.h>
#include <types.h>

// https://wiki.osdev.org/AC97

#define AC97_RESET 0x00
#define AC97_MASTER_VOLUME 0x02
#define AC97_AUX_OUT_VOLUME 0x04
#define AC97_MONO_VOLUME 0x06
#define AC97_MASTER_TONE 0x08

#define AC97_PC_BEEP_VOLUME 0x0A
#define AC97_PHONE_VOLUME 0x0C
#define AC97_MIC_VOLUME 0x0E
#define AC97_LINEIN_VOLUME 0x10
#define AC97_CD_VOLUME 0x12
#define AC97_VIDEO_VOLUME 0x14
#define AC97_AUXIN_VOLUME 0x16
#define AC97_PCMOUT_VOLUME 0x18
#define AC97_RECORD_SELECT 0x1A
#define AC97_RECORD_GAIN 0x1C
#define AC97_RECORD_GAIN_MIC 0x1E
#define AC97_GENERAL_PURPOSE 0x20
#define AC97_3D_CONTROL 0x22

struct ac97_devicedata {
    uint64_t base;
} __attribute__((packed));

void ac97_handle_irq(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t device_initAC97(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct ac97_devicedata* device_data = (struct ac97_devicedata*)dev->device_data;
    device_data->base = pci_calcbar(dev->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", dev->description, dev->pci->irq,
            dev->pci->vendor_id, dev->pci->device_id, device_data->base, dev->name);
    interrupt_router_register_interrupt_handler(dev->pci->irq, &ac97_handle_irq);
    return 1;
}

void AC97PCISearchCB(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &device_initAC97;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = DSP;
    devicemgr_set_device_description(deviceinstance, "Intel 82801AA AC97");
    /*
     * device api
     */
    struct deviceapi_dsp* api = (struct deviceapi_dsp*)kmalloc(sizeof(struct deviceapi_dsp));
    deviceinstance->api = api;
    /*
     * the device_data
     */
    struct ac97_devicedata* device_data = (struct ac97_devicedata*)kmalloc(sizeof(struct ac97_devicedata));
    device_data->base = 0;
    deviceinstance->device_data = device_data;
    /*
     * register
     */
    devicemgr_register_device(deviceinstance);
}

void ac97_devicemgr_register_devices() {
    pci_devicemgr_search_device(PCI_CLASS_MULTIMEDIA, 0x01, 0x8086, 0x2415, &AC97PCISearchCB);
}
