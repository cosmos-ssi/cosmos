//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
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
#define FONT_NAME "ter-u12b.bdf"

void gui_init() {
    struct device* bga = devicemgr_find_device(VGA_DEVICE_NAME);
    if (0 != bga) {
        gui_state = (struct gui_state_data*)kmalloc(sizeof(struct gui_state_data));
        gui_state->canvas = canvas_new(bga);
        gui_state->background_color = 0x97cee8;  // light blue
        gui_state->font = bdf_new();
        gui_state->windows = arraylist_new();
        bdf_load(gui_state->font, INIT_DEVICE_NAME, FONT_NAME);
        //   canvas_dump(gui_state->canvas);

        /*
        * add some windows
        */
        gui_new_window(400, 400, 100, 100);
        gui_new_window(50, 50, 300, 300);

    } else {
        kprintf("Unable to find video device %s for GUI\n", VGA_DEVICE_NAME);
    }
}

void gui_draw() {
    ASSERT_NOT_NULL(gui_state);
    ASSERT_NOT_NULL(gui_state->canvas);

    canvas_clear(gui_state->canvas, gui_state->background_color);
    canvas_blt(gui_state->canvas);
    //  canvas_dump(gui_state->canvas);
    //  struct bmp* wallpaper = bmp_new();
    //  if (0 != wallpaper) {
    //      bmp_load(wallpaper, INIT_DEVICE_NAME, WALLPAPER_NAME);
    //      kprintf("wallpaper size %llu\n", wallpaper->buffer_size);
    //  }
    for (uint32_t i = 0; i < arraylist_count(gui_state->windows); i++) {
        struct window* w = (struct window*)arraylist_get(gui_state->windows, i);
        ASSERT_NOT_NULL(w);
        window_render(w, gui_state->canvas);
    }
    canvas_blt(gui_state->canvas);
}

struct window* gui_new_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    struct window* ret = window_new(x, y, width, height);
    arraylist_add(gui_state->windows, ret);
    return ret;
}
