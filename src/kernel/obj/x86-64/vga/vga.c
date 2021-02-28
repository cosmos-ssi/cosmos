//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/pci/pci.h>
#include <obj/x86-64/vga/vga.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_vga.h>

// CRT control I/O ports
#define CRT_INDEX_REGISTER 0x3D4
#define CRT_DATA_REGISTER 0x3D5

// cursor control commands
#define CURSOR_LOCATION_HIBYTE 0x0E
#define CURSOR_LOCATION_LOBYTE 0x0F

char* vga_vga_text_mem_base = (char*)0xB8000;
char* vga_vga_graphic_mem_base = (char*)0xA0000;

// x-width and y-height are either characters for text modes, or pixels for graphics modes
struct vga_mode_params {
    uint16_t x_width;
    uint16_t y_height;
};

struct vga_devicedata {
    enum vga_video_mode video_active_mode;
    struct vga_mode_params vga_modes[VIDEO_MODE_MAX];
};

/*
 * perform device instance specific init here
 */
uint8_t vga_obj_init(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct vga_devicedata* object_data = (struct vga_devicedata*)dev->object_data;

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", dev->description, dev->pci->irq, dev->pci->vendor_id,
            dev->pci->device_id, dev->name);
    object_data->vga_modes[VIDEO_MODE_TEXT].x_width = 80;
    object_data->vga_modes[VIDEO_MODE_TEXT].y_height = 25;
    object_data->video_active_mode = VIDEO_MODE_TEXT;
    return 1;
}

// api
uint8_t vga_device_set_mode(struct object* dev, enum vga_video_mode mode) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    //   struct vga_devicedata* object_data = (struct vga_devicedata*)dev->object_data;

    ASSERT_NOT_NULL(dev);
    if (mode == VIDEO_MODE_TEXT) {
        return 1;
    } else {
        return 0;
    }
}

// api
void vga_device_scroll_text(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct vga_devicedata* object_data = (struct vga_devicedata*)dev->object_data;

    uint16_t i;
    uint16_t row_size;
    uint16_t screen_size;
    uint16_t last_row_loc;

    screen_size =
        (object_data->vga_modes[VIDEO_MODE_TEXT].x_width * object_data->vga_modes[VIDEO_MODE_TEXT].y_height * 2);

    row_size = object_data->vga_modes[VIDEO_MODE_TEXT].x_width * 2;

    last_row_loc = screen_size - row_size;

    // start at the beginning of the second row (row 1 in zero-based numbering) and copy each value (character and attribute) to the location
    // x_width * 2 positions before it.
    for (i = row_size; i < screen_size; i++) {
        vga_vga_text_mem_base[i - row_size] = vga_vga_text_mem_base[i];
    }

    // and now we blank the last line
    for (i = 0; i < row_size; i++) {
        vga_vga_text_mem_base[last_row_loc + i] = '\0';
    }

    return;
}

void vga_cursor_set_position(uint16_t loc) {
    asm_out_b(CRT_INDEX_REGISTER, CURSOR_LOCATION_HIBYTE);
    asm_out_b(CRT_DATA_REGISTER, (uint8_t)((loc >> 8) & 0xFF));

    asm_out_b(CRT_INDEX_REGISTER, CURSOR_LOCATION_LOBYTE);
    asm_out_b(CRT_DATA_REGISTER, (uint8_t)(loc & 0xFF));
}

// api
uint8_t vga_device_write_text(struct object* dev, const char* txt, uint8_t start_row, uint8_t start_col, uint8_t attrib,
                              enum vga_text_color fg_color, enum vga_text_color bg_color) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct vga_devicedata* object_data = (struct vga_devicedata*)dev->object_data;

    // ignore attrib for now, but I went ahead and put it in the API to minimize breaking things when I add support for it
    char* startpoint;
    uint64_t i = 0;  // I mean, it probably doesn't need to be 64 bits, but just to avoid unanticipated issues...

    startpoint = (char*)vga_vga_text_mem_base +
                 ((start_row * object_data->vga_modes[VIDEO_MODE_TEXT].x_width * 2) + (start_col * 2));

    while (txt[i]) {
        // make sure we don't write past the end of the vga text mode memory area
        if (startpoint >= (vga_vga_text_mem_base + (object_data->vga_modes[VIDEO_MODE_TEXT].x_width *
                                                    object_data->vga_modes[VIDEO_MODE_TEXT].y_height * 2))) {
            return 0;
        }

        *startpoint = txt[i];

        startpoint++;

        *startpoint = (bg_color << 4) + fg_color;

        startpoint++;

        i++;
    }

    vga_cursor_set_position((uint16_t)((uint64_t)(startpoint - vga_vga_text_mem_base) / 2));

    return 1;
}

// api
uint8_t vga_device_query_resolution(struct object* dev, uint16_t* x, uint16_t* y) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->object_data);
    struct vga_devicedata* object_data = (struct vga_devicedata*)dev->object_data;

    *x = object_data->vga_modes[object_data->video_active_mode].x_width;
    *y = object_data->vga_modes[object_data->video_active_mode].y_height;
    return 1;
}

void vga_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* deviceinstance = objectmgr_new_object();
    deviceinstance->init = &vga_obj_init;
    deviceinstance->pci = dev;
    deviceinstance->devicetype = VGA;
    objectmgr_set_object_description(deviceinstance, "QEMU/Bochs VBE Framebuffer");
    objectmgr_register_object(deviceinstance);
    /*
     * device api
     */
    struct objecttype_vga* api = (struct objecttype_vga*)kmalloc(sizeof(struct objecttype_vga));
    api->query_resolution = &vga_device_query_resolution;
    api->scroll_text = &vga_device_scroll_text;
    api->set_mode = &vga_device_set_mode;
    api->write_text = &vga_device_write_text;
    deviceinstance->api = api;
    /*
     * device data
     */
    struct vga_devicedata* object_data = (struct vga_devicedata*)kmalloc(sizeof(struct vga_devicedata));
    object_data->video_active_mode = 0;
    deviceinstance->object_data = object_data;
}

/**
 * find all Display devices and register them
 */
void vga_objectmgr_register_objects() {
    pci_objectmgr_search_devicetype(PCI_CLASS_DISPLAY, PCI_DISPLAY_SUBCLASS_VGA, &vga_search_cb);
}
