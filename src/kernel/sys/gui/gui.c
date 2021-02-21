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
#include <sys/video/canvas.h>
#include <sys/video/video_util.h>

struct gui_state_data* gui_state;

#define VGA_DEVICE_NAME "bga0"

void gui_init() {
    struct device* bga = devicemgr_find_device(VGA_DEVICE_NAME);
    if (0 != bga) {
        gui_state = (struct gui_state_data*)kmalloc(sizeof(struct gui_state_data));
        gui_state->canvas = canvas_new(bga);
        canvas_dump(gui_state->canvas);
        /*
        * clear
        */
        video_util_clear(bga, 0xEEEEEE);
    } else {
        kprintf("Unable to find video device %s for GUI\n", VGA_DEVICE_NAME);
    }
}
