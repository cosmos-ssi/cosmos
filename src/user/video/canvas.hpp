//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* this is a video canvas.  We draw on it, and then blt it to the video memory.  
* canvas uses video_util and presumes a linear frame buffer (LFB)
* for now the canvas presumes 800x600, 24bpp
*/
#ifndef _CANVAS_HPP
#define _CANVAS_HPP

#include <object/objects.hpp>
#include <string.hpp>
#include <types.h>

class object;
class psf1_font;
class bmp;
class tga;

class Canvas {
  private:
    uint8_t* buffer;
    struct BGAObject* bga;
    uint32_t buffer_size;
    uint32_t bytes_per_pixel;
    struct syscall_objectinterface_resolution resolution;

    uint32_t abs_diff(uint32_t x1, uint32_t x2);
    uint32_t pixel_offset(uint32_t x, uint32_t y);
    void draw_sloped_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);

  public:
    Canvas(BGAObject* bga);
    ~Canvas();
    /*
    * draw a line from x0,y0 to x1,y1
    */
    void draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);
    void draw_pixel(uint32_t x, uint32_t y, uint32_t rgb);
    void dump();
    void clear(uint32_t rgb);

    /*
    * this fills a rectangle with x0,y0 the upper left and x1,y1 the lower right
    */
    void fill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);
    /*
    * blt the canvas to the underlying BGA device
    */
    void blt();

    /*
    * draw a letter using psf1 font
    */
    void draw_letter(psf1_font* font, uint32_t x, uint32_t y, uint8_t c, uint32_t color);

    /*
    * draw a string
    */
    void draw_letters(psf1_font* font, uint32_t x, uint32_t y, uint8_t* str, uint32_t color);

    /*
    * draw a bitmap
    */
    void draw_bitmap(bmp* bitmap, uint32_t x, uint32_t y);

    /*
    * draw a targa
    */
    void draw_targa(tga* targa, uint32_t x, uint32_t y);
};

#endif
