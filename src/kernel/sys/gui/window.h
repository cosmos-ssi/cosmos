//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _WINDOW_H
#define _WINDOW_H

#include <types.h>

struct canvas;

struct window {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t background_color;
    uint32_t border_color;
    uint32_t titlebar_height;
    uint32_t title_background_color;
};

struct window* window_new(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void window_set_position(struct window* wind, uint32_t x, uint32_t y);
void window_set_dimensions(struct window* wind, uint32_t width, uint32_t height);

void window_render(struct window* w, struct canvas* cvs);

#endif
