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
#ifndef _CANVAS_H
#define _CANVAS_H

#include <sys/objecttype/objecttype_bga.h>
#include <types.h>

struct device;
struct psf1_font;
struct bmp;

struct canvas {
    uint8_t* buffer;
    struct device* dev;
    uint32_t buffer_size;
    uint32_t bytes_per_pixel;
    struct objecttype_resolution resolution;
};

struct canvas* canvas_new(struct device* dev);
void canvas_delete(struct canvas* cvs);
/*
* draw a line from x0,y0 to x1,y1
*/
void canvas_draw_line(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);
void canvas_draw_pixel(struct canvas* cvs, uint32_t x, uint32_t y, uint32_t rgb);
void canvas_dump(struct canvas* cvs);
void canvas_clear(struct canvas* cvs, uint32_t rgb);

/*
* this fills a rectangle with x0,y0 the upper left and x1,y1 the lower right
*/
void canvas_fill(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);
/*
* blt the canvas to the underlying BGA device
*/
void canvas_blt(struct canvas* cvs);

/*
* draw a letter using psf1 font
*/
void canvas_draw_letter(struct canvas* cvs, struct psf1_font* font, uint32_t x, uint32_t y, uint8_t c, uint32_t color);

/*
* draw a string
*/
void canvas_draw_letters(struct canvas* cvs, struct psf1_font* font, uint32_t x, uint32_t y, uint8_t* str,
                         uint32_t color);

/*
* draw a bitmap
*/
void canvas_draw_bitmap(struct canvas* cvs, struct bmp* bitmap, uint32_t x, uint32_t y);

#endif
