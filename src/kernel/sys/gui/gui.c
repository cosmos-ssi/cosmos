//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/gui/bdf.h>
#include <sys/gui/bmp.h>
#include <sys/gui/gui.h>
#include <sys/gui/window.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/video/canvas.h>
#include <sys/video/video_util.h>

struct gui_state_data* gui_state;

#define VGA_DEVICE_NAME "bga0"
#define INIT_DEVICE_NAME "fs2"
#define WALLPAPER_NAME "ocean.bmp"

void gui_init() {
    struct device* bga = devicemgr_find_device(VGA_DEVICE_NAME);
    if (0 != bga) {
        gui_state = (struct gui_state_data*)kmalloc(sizeof(struct gui_state_data));
        gui_state->canvas = canvas_new(bga);
        gui_state->background_color = 0x97cee8;  // light blue
        gui_state->font = bdf_new();
        bdf_load(gui_state->font, "fs2", "ter-u12b.bdf");
        //   canvas_dump(gui_state->canvas);
    } else {
        kprintf("Unable to find video device %s for GUI\n", VGA_DEVICE_NAME);
    }
}

void gui_draw() {
    ASSERT_NOT_NULL(gui_state);
    ASSERT_NOT_NULL(gui_state->canvas);

    canvas_clear(gui_state->canvas, gui_state->background_color);
    canvas_blt(gui_state->canvas);
    canvas_dump(gui_state->canvas);
    //  struct bmp* wallpaper = bmp_new();
    //  if (0 != wallpaper) {
    //      bmp_load(wallpaper, INIT_DEVICE_NAME, WALLPAPER_NAME);
    //      kprintf("wallpaper size %llu\n", wallpaper->buffer_size);
    //  }
    struct window* w1 = window_new(400, 400, 100, 100);
    struct window* w2 = window_new(50, 50, 300, 300);

    window_render(w1, gui_state->canvas);
    window_render(w2, gui_state->canvas);

    canvas_blt(gui_state->canvas);
}
