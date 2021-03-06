//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://forum.osdev.org/viewtopic.php?f=1&t=30884

#include <obj/x86-64/bga/bga.h>
#include <obj/x86-64/pci/devicetree.h>
#include <obj/x86-64/pci/pci_device.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_bga.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

#define VBE_DISPI_BANK_SIZE_KB 64

#define VBE_DISPI_ID0 0xB0C0
#define VBE_DISPI_ID1 0xB0C1
#define VBE_DISPI_ID2 0xB0C2
#define VBE_DISPI_ID3 0xB0C3
#define VBE_DISPI_ID4 0xB0C4
#define VBE_DISPI_ID5 0xB0C5

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA 0x01CF

#define VBE_DISPI_INDEX_ID 0x0

#define VBE_DISPI_INDEX_XRES 0x1
#define VBE_DISPI_INDEX_YRES 0x2
#define VBE_DISPI_MAX_XRES 1024
#define VBE_DISPI_MAX_YRES 768

#define VBE_DISPI_INDEX_BPP 0x3
#define VBE_DISPI_INDEX_ENABLE 0x4
#define VBE_DISPI_INDEX_BANK 0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH 0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 0x7
#define VBE_DISPI_INDEX_X_OFFSET 0x8
#define VBE_DISPI_INDEX_Y_OFFSET 0x9

#define VBE_DISPI_DISABLED 0x00
#define VBE_DISPI_ENABLED 0x01
#define VBE_DISPI_GETCAPS 0x02
#define VBE_DISPI_8BIT_DAC 0x20
#define VBE_DISPI_LFB_ENABLED 0x40
#define VBE_DISPI_NOCLEARMEM 0x80

void bga_set_resolution(struct object* obj, struct objectinterface_resolution* resolution);

struct bga_objectdata {
    uint64_t lfb_physical;
    uint64_t lfb_virtual;
    struct objectinterface_resolution resolution;
};

uint32_t bga_buffer_size(struct bga_objectdata* object_data) {
    ASSERT_NOT_NULL(object_data);
    return (object_data->resolution.height * object_data->resolution.width) * (object_data->resolution.color_depth / 8);
}

void bga_write_register(uint16_t index_value, uint16_t data_value) {
    asm_out_w(VBE_DISPI_IOPORT_INDEX, index_value);
    asm_out_w(VBE_DISPI_IOPORT_DATA, data_value);
}

uint16_t bga_read_register(uint16_t index_value) {
    asm_out_w(VBE_DISPI_IOPORT_INDEX, index_value);
    return asm_in_w(VBE_DISPI_IOPORT_DATA);
}

uint8_t bga_is_available(void) {
    return (bga_read_register(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
}

void bga_set_video_mode(uint32_t width, uint32_t height, uint32_t bit_depth, uint8_t use_linear_frame_buffer,
                        uint8_t clear_video_memory) {
    //  kprintf("w %llu, h %llu\n", width, height);
    bga_write_register(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    bga_write_register(VBE_DISPI_INDEX_XRES, width);
    bga_write_register(VBE_DISPI_INDEX_YRES, height);
    bga_write_register(VBE_DISPI_INDEX_BPP, bit_depth);
    bga_write_register(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
                                                   (use_linear_frame_buffer ? VBE_DISPI_LFB_ENABLED : 0) |
                                                   (clear_video_memory ? 0 : VBE_DISPI_NOCLEARMEM));
}

void bga_set_bank(uint16_t bank_number) {
    bga_write_register(VBE_DISPI_INDEX_BANK, bank_number);
}

/*
 * perform device instance specific init here
 */
uint8_t bga_obj_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->pci);
    ASSERT_NOT_NULL(obj->object_data);
    struct bga_objectdata* object_data = (struct bga_objectdata*)obj->object_data;
    /* 
    * lfb
    */
    object_data->lfb_physical = obj->pci->bars[0] & 0xFFFFFFF0;
    object_data->lfb_virtual = (uint64_t)CONV_PHYS_ADDR(object_data->lfb_physical);

    /*
    * screen params
    */
    struct objectinterface_resolution resolution;
    resolution.width = 1280;
    resolution.height = 1024;
    resolution.color_depth = 32;
    bga_set_resolution(obj, &resolution);

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX at lfb (physical) %#llX lfb (virtual) %#llX (%s)\n",
            obj->description, obj->pci->irq, obj->pci->vendor_id, obj->pci->device_id, object_data->lfb_physical,
            object_data->lfb_virtual, obj->name);
    //   kprintf("bga_buffer_size : %#llX\n", bga_buffer_size(object_data));
    return 1;
}

uint32_t bga_get_buffersize(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->pci);
    ASSERT_NOT_NULL(obj->object_data);
    return bga_buffer_size(obj->object_data);
}

void bga_blt(struct object* obj, uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->pci);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(buffer);
    ASSERT(buffer_size = bga_buffer_size(obj->object_data));
    struct bga_objectdata* object_data = (struct bga_objectdata*)obj->object_data;
    memcpy((uint8_t*)object_data->lfb_virtual, buffer, buffer_size);
}

void bga_get_resolution(struct object* obj, struct objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->pci);
    ASSERT_NOT_NULL(obj->object_data);
    struct bga_objectdata* object_data = (struct bga_objectdata*)obj->object_data;
    ASSERT_NOT_NULL(resolution);
    resolution->color_depth = object_data->resolution.color_depth;
    resolution->height = object_data->resolution.height;
    resolution->width = object_data->resolution.width;
}

void bga_set_resolution(struct object* obj, struct objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->pci);
    ASSERT_NOT_NULL(obj->object_data);
    struct bga_objectdata* object_data = (struct bga_objectdata*)obj->object_data;
    ASSERT_NOT_NULL(resolution);
    object_data->resolution.color_depth = resolution->color_depth;
    object_data->resolution.height = resolution->height;
    object_data->resolution.width = resolution->width;
    bga_set_video_mode(object_data->resolution.width, object_data->resolution.height,
                       object_data->resolution.color_depth, 1, 1);
    memset((uint8_t*)object_data->lfb_virtual, 0x00,
           (object_data->resolution.width * object_data->resolution.height) *
               (object_data->resolution.color_depth / 8));
}

void bga_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &bga_obj_init;
    objectinstance->pci = dev;
    objectinstance->objectype = OBJECT_TYPE_BGA;
    objectmgr_set_object_description(objectinstance, "QEMU/Bochs BGA Framebuffer");
    objectmgr_register_object(objectinstance);
    /*
     * device api
     */
    struct objectinterface_bga* api = (struct objectinterface_bga*)kmalloc(sizeof(struct objectinterface_bga));
    api->get_resolution = &bga_get_resolution;
    api->set_resolution = &bga_set_resolution;
    api->get_buffersize = &bga_get_buffersize;
    api->blt = &bga_blt;
    objectinstance->api = api;

    /*
     * device data
     */
    struct bga_objectdata* object_data = (struct bga_objectdata*)kmalloc(sizeof(struct bga_objectdata));
    memzero((uint8_t*)object_data, sizeof(struct bga_objectdata));
    object_data->lfb_physical = 0;
    object_data->lfb_virtual = 0;
    objectinstance->object_data = object_data;
}

/**
 * find all Display devices and register them
 */
void bga_objectmgr_register_objects() {
    pci_objectmgr_search_objectype(PCI_CLASS_DISPLAY, PCI_DISPLAY_SUBCLASS_VGA, &bga_search_cb);
}
