//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* GUI uses the canvas object to draw the GUI
*/
#ifndef _GUI_HPP
#define _GUI_HPP

#include <types.h>
class tga;
class bmp;
class Window;

class GUI {
  public:
    uint32_t background_color;
    Canvas* canvas;
    psf1_font* font;
    ArrayList<Window>* windows;
    tga* background_image;

    void init();
    void draw();

    Window* new_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};

extern GUI* gui;

#endif
