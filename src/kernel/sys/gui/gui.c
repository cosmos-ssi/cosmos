//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/gui/gui.h>
#include <sys/gui/window.h>
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
        //   canvas_dump(gui_state->canvas);
    } else {
        kprintf("Unable to find video device %s for GUI\n", VGA_DEVICE_NAME);
    }
}

void gui_draw() {
    ASSERT_NOT_NULL(gui_state);
    ASSERT_NOT_NULL(gui_state->canvas);

    canvas_clear(gui_state->canvas, 0xAAAA77);

    //  canvas_draw_pixel(gui_state->canvas, 0, 0, 0x111111);
    canvas_blt(gui_state->canvas);

    //    canvas_draw_line(gui_state->canvas, 0, 0, 300, 300, 0x222222);
    //    canvas_blt(gui_state->canvas);

    struct window* w = window_new(50, 50, 300, 300);
    window_render(w, gui_state->canvas);
    canvas_blt(gui_state->canvas);
}
