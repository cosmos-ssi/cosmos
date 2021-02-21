//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/devicemgr/devicemgr.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/video/canvas.h>
#include <sys/video/rgb.h>
#include <sys/video/video_util.h>

struct canvas* canvas_new(struct device* dev) {
    ASSERT_NOT_NULL(dev);
    struct canvas* ret = (struct canvas*)kmalloc(sizeof(struct canvas));
    /*
    * useful state data
    */
    ret->buffer_size = video_util_get_buffersize(dev);
    ret->color_depth = video_util_get_colordepth(dev);
    ASSERT(ret->color_depth == 24);
    ret->height = video_util_get_height(dev);
    ASSERT(ret->height == 600);
    ret->width = video_util_get_width(dev);
    ASSERT(ret->width == 800);
    ret->buffer = kmalloc(ret->buffer_size);
    ret->dev = dev;
    return ret;
}

uint32_t canvas_pixel_offset(struct canvas* cvs, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(cvs);
    ASSERT(x < cvs->width);
    ASSERT(y < cvs->height);
    return 3 * ((cvs->width * y) + x);
}

void canvas_dump(struct canvas* cvs) {
    ASSERT_NOT_NULL(cvs);

    kprintf("width %llu, height %llu depth %llu size %llu\n", cvs->width, cvs->height, cvs->color_depth,
            cvs->buffer_size);
}

void canvas_blt(struct canvas* cvs) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->buffer);
    ASSERT_NOT_NULL(cvs->dev);
    video_util_blt(cvs->dev, cvs->buffer, cvs->buffer_size);
}

void canvas_clear(struct canvas* cvs, uint32_t rgb) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->buffer);
    ASSERT_NOT_NULL(cvs->dev);
    ASSERT_NOT_NULL(cvs->width);
    ASSERT_NOT_NULL(cvs->height);
    ASSERT_NOT_NULL(cvs->buffer_size);
    ASSERT_NOT_NULL(cvs->color_depth);

    /*
    * components
    */
    struct rgb_components components;
    rgb_components(rgb, &components);
    //  kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, components.r, components.g, components.b);
    /*
    * paint
    */
    for (uint32_t i = 0; i < (cvs->width * cvs->height * 3); i += 3) {
        cvs->buffer[i] = components.b;
        cvs->buffer[i + 1] = components.g;
        cvs->buffer[i + 2] = components.r;
    }
}

void canvas_draw_pixel(struct canvas* cvs, uint32_t x, uint32_t y, uint32_t rgb) {
    ASSERT_NOT_NULL(cvs);
    uint32_t offset = canvas_pixel_offset(cvs, x, y);
    //  kprintf("offset %#llX,\n", offset);
    struct rgb_components components;
    rgb_components(rgb, &components);
    //  kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, components.r, components.g, components.b);

    cvs->buffer[offset] = components.b;
    cvs->buffer[offset + 1] = components.g;
    cvs->buffer[offset + 2] = components.r;
}

uint32_t abs_diff(uint32_t x1, uint32_t x2) {
    if (x2 > x1) {
        return x2 - x1;
    }
    return x1 - x2;
}

/*
* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
*/
void canvas_draw_sloped_line(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    uint32_t dx = abs_diff(x0, x1);
    uint32_t dy = abs_diff(y0, y1);
    uint32_t p = 2 * dy - dx;
    uint32_t twoDy = 2 * dy, twoDyDx = 2 * (dy - dx);
    uint32_t x, y, xEnd;
    if (x0 > x1) {
        x = x1;
        y = y1;
        xEnd = x0;
    } else {
        x = x0;
        y = y0;
        xEnd = x1;
    }
    canvas_draw_pixel(cvs, x, y, rgb);

    while (x < xEnd) {
        x++;
        if (p < 0)
            p += twoDy;
        else {
            y++;
            p += twoDyDx;
        }
        canvas_draw_pixel(cvs, x, y, rgb);
    }
}

void canvas_draw_line(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    kprintf("x0 %llu, y0 %llu, x1 %llu, y1 %llu\n", x0, y0, x1, y1);

    if (x0 == x1) {
        if (y1 > y0) {
            for (uint32_t i = y0; i < y1; i++) {
                canvas_draw_pixel(cvs, x0, i, rgb);
            }
        } else {
            for (uint32_t i = y1; i < y0; i++) {
                canvas_draw_pixel(cvs, x0, i, rgb);
            }
        }
    } else if (y0 == y1) {
        if (x1 > x0) {
            for (uint32_t i = x0; i < x1; i++) {
                canvas_draw_pixel(cvs, i, y0, rgb);
            }
        } else {
            for (uint32_t i = x1; i < x0; i++) {
                canvas_draw_pixel(cvs, i, y0, rgb);
            }
        }
    } else {
        canvas_draw_sloped_line(cvs, x0, y0, x1, y1, rgb);
    }
}
