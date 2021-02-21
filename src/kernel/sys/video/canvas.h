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

#include <types.h>

struct device;

struct canvas {
    uint8_t* buffer;
    struct device* dev;
    uint32_t width;
    uint32_t height;
    uint32_t color_depth;
    uint32_t buffer_size;
};

struct canvas* canvas_new(struct device* dev);
void canvas_draw_line(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb);
void canvas_draw_pixel(struct canvas* cvs, uint32_t x, uint32_t y, uint32_t rgb);
void canvas_dump(struct canvas* cvs);
void canvas_clear(struct canvas* cvs, uint32_t rgb);

/*
* blt the canvas to the underlying BGA device
*/
void canvas_blt(struct canvas* cvs);

#endif
