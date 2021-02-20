//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://forum.osdev.org/viewtopic.php?f=1&t=30884

#include <dev/x86-64/bga/bga.h>
#include <dev/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_bga.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/mm/pagetables.h>

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

struct bga_devicedata {
    uint64_t lfb_physical;
    uint64_t lfb_virtual;
    uint32_t width;
    uint32_t height;
    uint32_t bit_depth;
};

uint32_t bga_buffer_size(struct bga_devicedata* device_data) {
    ASSERT_NOT_NULL(device_data);
    return (device_data->height * device_data->width) * (device_data->bit_depth / 8);
}

// *3 for 24 bit color
uint32_t bga_get_offset(struct bga_devicedata* device_data, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(device_data);
    return (device_data->width * y * 3) + x;
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

void bga_clear(struct device* dev) {
    ASSERT_NOT_NULL(dev);
}

/*
 * perform device instance specific init here
 */
uint8_t bga_device_init(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->pci);
    ASSERT_NOT_NULL(dev->device_data);
    struct bga_devicedata* device_data = (struct bga_devicedata*)dev->device_data;
    /* 
    * lfb
    */
    device_data->lfb_physical = dev->pci->bars[0] & 0xFFFFFFF0;
    device_data->lfb_virtual = (uint64_t)CONV_PHYS_ADDR(device_data->lfb_physical);

    /*
    * screen params
    */
    device_data->width = 800;
    device_data->height = 600;
    device_data->bit_depth = 24;

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX at lfb(physical) %#llX lfb(virtual) %#llX (%s)\n",
            dev->description, dev->pci->irq, dev->pci->vendor_id, dev->pci->device_id, device_data->lfb_physical,
            device_data->lfb_virtual, dev->name);
    kprintf("bga_buffer_size : %#llX\n", bga_buffer_size(device_data));

    //  kprintf("bar0 : %#llX\n", dev->pci->bars[0]);
    //  kprintf("bar1 : %#llX\n", dev->pci->bars[1]);
    //  kprintf("bar2 : %#llX\n", dev->pci->bars[2]);
    //  kprintf("bar3 : %#llX\n", dev->pci->bars[3]);
    //  kprintf("bar4 : %#llX\n", dev->pci->bars[4]);
    //  kprintf("bar5 : %#llX\n", dev->pci->bars[5]);

    //  if (bga_is_available()) {
    bga_set_video_mode(device_data->width, device_data->height, device_data->bit_depth, 1, 1);
    //  }

    // color
    uint64_t buffer_size = bga_buffer_size(device_data);
    for (uint32_t i = 0; i < buffer_size; i += 3) {
        //   uint64_t address = device_data->lfb_virtual + i;
        //  kprintf("address : %#llX\n", address);

        //  asm_out_b(address, 0x00);
    }

    return 1;
}

void bga_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct device* deviceinstance = devicemgr_new_device();
    deviceinstance->init = &bga_device_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = BGA;
    devicemgr_set_device_description(deviceinstance, "QEMU/Bochs BGA Framebuffer");
    devicemgr_register_device(deviceinstance);
    /*
     * device api
     */
    struct deviceapi_bga* api = (struct deviceapi_bga*)kmalloc(sizeof(struct deviceapi_bga));
    api->clear = &bga_clear;
    deviceinstance->api = api;

    /*
     * device data
     */
    struct bga_devicedata* device_data = (struct bga_devicedata*)kmalloc(sizeof(struct bga_devicedata));
    device_data->lfb_physical = 0;
    device_data->lfb_virtual = 0;
    device_data->width = 0;
    device_data->height = 0;
    device_data->bit_depth = 0;
    deviceinstance->device_data = device_data;
}

/**
 * find all Display devices and register them
 */
void bga_devicemgr_register_devices() {
    pci_devicemgr_search_devicetype(PCI_CLASS_DISPLAY, PCI_DISPLAY_SUBCLASS_VGA, &bga_search_cb);
}
