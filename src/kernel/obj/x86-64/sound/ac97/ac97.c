/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/pci/pci.h>
#include <obj/x86-64/sound/ac97/ac97.h>
#include <sys/asm/asm.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_dsp.h>

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

struct ac97_objectdata {
    uint64_t base;
} __attribute__((packed));

void ac97_handle_irq(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
}

/*
 * perform device instance specific init here
 */
uint8_t obj_initAC97(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct ac97_objectdata* object_data = (struct ac97_objectdata*)obj->object_data;
    object_data->base = pci_calcbar(obj->pci);
    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);
    interrupt_router_register_interrupt_handler(obj->pci->irq, &ac97_handle_irq);
    return 1;
}

void AC97PCISearchCB(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &obj_initAC97;
    objectinstance->pci = dev;
    objectinstance->objectype = DSP;
    objectmgr_set_object_description(objectinstance, "Intel 82801AA AC97");
    /*
     * device api
     */
    struct objectinterface_dsp* api = (struct objectinterface_dsp*)kmalloc(sizeof(struct objectinterface_dsp));
    objectinstance->api = api;
    /*
     * the object_data
     */
    struct ac97_objectdata* object_data = (struct ac97_objectdata*)kmalloc(sizeof(struct ac97_objectdata));
    object_data->base = 0;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

void ac97_objectmgr_register_objects() {
    pci_objectmgr_search_device(PCI_CLASS_MULTIMEDIA, 0x01, 0x8086, 0x2415, &AC97PCISearchCB);
}
