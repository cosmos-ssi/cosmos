//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/gui/gui.h>
#include <sys/gui/window.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/video/bmp.h>
#include <sys/video/canvas.h>
#include <sys/video/psf.h>

struct gui_state_data* gui_state;

#define VGA_DEVICE_NAME "bga0"
#define INIT_DEVICE_NAME "fs2"
#define WALLPAPER_NAME "cosmos.bmp"
#define FONT_NAME "zap-vga16.psf"

void gui_init() {
    struct object* bga = objectmgr_find_object_by_name(VGA_DEVICE_NAME);
    if (0 != bga) {
        gui_state = (struct gui_state_data*)kmalloc(sizeof(struct gui_state_data));
        gui_state->canvas = canvas_new(bga);
        gui_state->background_color = 0x97cee8;  // light blue
        gui_state->font = psf_load(INIT_DEVICE_NAME, FONT_NAME);
        gui_state->windows = arraylist_new();
        gui_state->background_image = bmp_load(INIT_DEVICE_NAME, WALLPAPER_NAME);
        // canvas_dump(gui_state->canvas);
        //    psf_dump(gui_state->font);

        //  psf_character(gui_state->font, 0);
        //  psf_character(gui_state->font, 10);
        //  psf_character(gui_state->font, 256);

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
    if (0 != gui_state->background_image) {
        canvas_draw_bitmap(gui_state->canvas, gui_state->background_image, 0, 0);
    }
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
