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
#include <string.hpp>
#include <window.hpp>

Window::Window(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    this->titlebar_height = 20;
    set_position(x, y);
    set_dimensions(width, height);
    this->background_color = 0xFFFFFF;
    this->title_background_color = 0xD8E1E7;
    this->border_color = 0xD8E1E7;
    this->title_font_color = 0x2A2E29;
    String::strncpy(this->title, (uint8_t*)"CosmOS", WINDOW_MAX_TITLE);
}

void Window::render(Canvas* cvs) {
    ASSERT_NOT_NULL(cvs);
    //  kprintf("x %llu, y %llu, width %llu, height %llu\n", wind->x, wind->y, wind->width, wind->height);

    cvs->draw_line(this->x, this->y, this->x + this->width, this->y, this->border_color);
    cvs->draw_line(this->x, this->y + this->titlebar_height, this->x + this->width, this->y + this->titlebar_height,
                   this->border_color);
    cvs->draw_line(this->x, this->y + this->height, this->x + this->width, this->y + this->height, this->border_color);
    cvs->draw_line(this->x, this->y, this->x, this->y + this->height, this->border_color);
    cvs->draw_line(this->x + this->width, this->y, this->x + this->width, this->y + this->height, this->border_color);

    // fill titlebar
    cvs->fill(this->x + 1, this->y + this->titlebar_height + 1, this->x + this->width - 1, this->y + this->height - 1,
              this->background_color);
    // fill body
    cvs->fill(this->x + 1, this->y + 1, this->x + this->width - 1, this->y + this->titlebar_height - 1,
              this->title_background_color);
    // draw title
    cvs->draw_letters(gui->font, this->x + 2, this->y + 2, this->title, this->title_font_color);
}

void Window::set_position(uint32_t x, uint32_t y) {
    this->x = x;
    this->y = y;
}

void Window::set_dimensions(uint32_t width, uint32_t height) {
    this->height = height;
    if (this->height < this->titlebar_height) {
        this->height = this->titlebar_height;
    }
    this->width = width;
}
