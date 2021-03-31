//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}

#include <arraylist.hpp>
#include <assert.hpp>
#include <canvas.hpp>
#include <gui.hpp>
#include <psf.hpp>
#include <string.hpp>
#include <window.hpp>

GUI* gui;

#define VGA_DEVICE_NAME "bga0"
#define INIT_DEVICE_NAME "fs0"
#define WALLPAPER_NAME "19921.tga"
#define FONT_NAME "zap-vg~1.psf"

void GUI::init() {
    // TODO
    BGAObject* bga = 0;  // objectmgr_find_object_by_name(VGA_DEVICE_NAME);
    if (0 != bga) {
        this->canvas = new Canvas(bga);
        this->background_color = 0x97cee8;  // light blue
        this->font = new psf1_font((uint8_t*)INIT_DEVICE_NAME, (uint8_t*)FONT_NAME);
        ASSERT_NOT_NULL(this->font);
        this->windows = new ArrayList<Window>();

        //  gui_state->background_image = tga_load(INIT_DEVICE_NAME, WALLPAPER_NAME);
        //  ASSERT_NOT_NULL(gui_state->background_image);
        this->background_image = 0;
        // canvas_dump(gui_state->canvas);
        //    psf_dump(gui_state->font);

        //  psf_character(gui_state->font, 0);
        //  psf_character(gui_state->font, 10);
        //  psf_character(gui_state->font, 256);

        /*
        * add some windows
        */
        new_window(400, 400, 100, 100);
        new_window(50, 50, 300, 300);
    }
}

void GUI::draw() {
    ASSERT_NOT_NULL(this->canvas);

    this->canvas->clear(this->background_color);
    this->canvas->blt();
    if (0 != this->background_image) {
        this->canvas->draw_targa(this->background_image, 0, 0);
    }
    for (uint32_t i = 0; i < this->windows->getCount(); i++) {
        Window* w = this->windows->get(i);
        ASSERT_NOT_NULL(w);
        w->render(this->canvas);
    }
    this->canvas->blt();
}

Window* GUI::new_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    Window* ret = new Window(x, y, width, height);
    this->windows->add(ret);
    return ret;
}
