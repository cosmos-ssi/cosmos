//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/devicemgr/devicemgr.h>
#include <sys/gui/gui.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/video/video_util.h>

struct gui_state_data* gui_state;

#define VGA_DEVICE_NAME "bga0"

void gui_init() {
    struct device* bga = devicemgr_find_device(VGA_DEVICE_NAME);
    if (0 != bga) {
        /*
        * useful state data
        */
        gui_state = kmalloc(sizeof(struct gui_state_data));
        gui_state->buffer_size = video_util_get_buffersize(bga);
        gui_state->color_depth = video_util_get_colordepth(bga);
        gui_state->height = video_util_get_height(bga);
        gui_state->width = video_util_get_width(bga);
        gui_state->buffer = kmalloc(gui_state->buffer_size);
        /*
        * clear
        */
        video_util_clear(bga, 0xFFFF00);
    } else {
        kprintf("Unable to find video device %s for GUI\n", VGA_DEVICE_NAME);
    }
}
