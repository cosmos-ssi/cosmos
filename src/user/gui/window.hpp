//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <types.h>

class Canvas;

#define WINDOW_MAX_TITLE 128

struct Window {
  private:
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t background_color;
    uint32_t border_color;
    uint32_t titlebar_height;
    uint32_t title_background_color;
    uint32_t title_font_color;
    uint8_t title[WINDOW_MAX_TITLE];

  public:
    Window(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void set_position(uint32_t x, uint32_t y);
    void set_dimensions(uint32_t width, uint32_t height);
    void render(Canvas* cvs);
};

#endif
