//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/video/bmp.h>
#include <sys/video/canvas.h>
#include <sys/video/psf.h>
#include <sys/video/rgb.h>
#include <sys/video/video_util.h>

struct canvas* canvas_new(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct canvas* ret = (struct canvas*)kmalloc(sizeof(struct canvas));
    /*
    * useful state data
    */
    ret->buffer_size = video_util_get_buffersize(obj);
    kprintf("Canvas buffer size %#llX\n", ret->buffer_size);
    video_get_resolution(obj, &(ret->resolution));
    ret->buffer = kmalloc(ret->buffer_size);
    ret->obj = obj;
    ret->bytes_per_pixel = ret->resolution.color_depth / 8;
    return ret;
}

void canvas_delete(struct canvas* cvs) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->buffer);
    kfree(cvs->buffer);
    kfree(cvs);
}

uint32_t canvas_pixel_offset(struct canvas* cvs, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->resolution.width);
    ASSERT_NOT_NULL(cvs->resolution.height);
    ASSERT(x < cvs->resolution.width);
    ASSERT(y < cvs->resolution.height);
    ASSERT(cvs->bytes_per_pixel > 0);
    return cvs->bytes_per_pixel * ((cvs->resolution.width * y) + x);
}

void canvas_dump(struct canvas* cvs) {
    ASSERT_NOT_NULL(cvs);

    kprintf("width %llu, height %llu depth %llu size %llu\n", cvs->resolution.width, cvs->resolution.height,
            cvs->resolution.color_depth, cvs->buffer_size);
}

void canvas_blt(struct canvas* cvs) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->buffer);
    ASSERT_NOT_NULL(cvs->obj);
    video_util_blt(cvs->obj, cvs->buffer, cvs->buffer_size);
}

void canvas_clear(struct canvas* cvs, uint32_t rgb) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(cvs->buffer);
    ASSERT_NOT_NULL(cvs->obj);
    ASSERT_NOT_NULL(cvs->resolution.width);
    ASSERT_NOT_NULL(cvs->resolution.height);
    ASSERT_NOT_NULL(cvs->buffer_size);
    ASSERT_NOT_NULL(cvs->resolution.color_depth);

    /*
    * components
    */
    struct rgb_components components;
    rgb_components(rgb, &components);
    //  kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, components.r, components.g, components.b);
    /*
    * paint
    */
    for (uint32_t i = 0; i < (cvs->resolution.width * cvs->resolution.height * cvs->bytes_per_pixel);
         i += cvs->bytes_per_pixel) {
        cvs->buffer[i] = components.b;
        cvs->buffer[i + 1] = components.g;
        cvs->buffer[i + 2] = components.r;
    }
}

void canvas_draw_pixel(struct canvas* cvs, uint32_t x, uint32_t y, uint32_t rgb) {
    ASSERT_NOT_NULL(cvs);
    ASSERT(x < cvs->resolution.width);
    ASSERT(y < cvs->resolution.height);

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
    ASSERT_NOT_NULL(cvs);
    ASSERT(x0 < cvs->resolution.width);
    ASSERT(y0 < cvs->resolution.height);
    ASSERT(x1 < cvs->resolution.width);
    ASSERT(y1 < cvs->resolution.height);

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
    ASSERT_NOT_NULL(cvs);
    ASSERT(x0 < cvs->resolution.width);
    ASSERT(y0 < cvs->resolution.height);
    ASSERT(x1 < cvs->resolution.width);
    ASSERT(y1 < cvs->resolution.height);

    //    kprintf("x0 %llu, y0 %llu, x1 %llu, y1 %llu\n", x0, y0, x1, y1);

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

void canvas_fill(struct canvas* cvs, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    ASSERT_NOT_NULL(cvs);
    ASSERT(x0 < cvs->resolution.width);
    ASSERT(y0 < cvs->resolution.height);
    ASSERT(x1 < cvs->resolution.width);
    ASSERT(y1 < cvs->resolution.height);

    uint32_t x = 0;
    uint32_t xx = 0;
    uint32_t y = 0;
    uint32_t yy = 0;

    if (x1 > x0) {
        x = x0;
        xx = x1;
    } else {
        x = x1;
        xx = x0;
    }
    if (y1 > y0) {
        y = y0;
        yy = y1;
    } else {
        y = y1;
        yy = y0;
    }
    for (uint32_t i = x; i <= xx; i++) {
        for (uint32_t j = y; j <= yy; j++) {
            canvas_draw_pixel(cvs, i, j, rgb);
        }
    }
}

void canvas_draw_letters(struct canvas* cvs, struct psf1_font* font, uint32_t x, uint32_t y, uint8_t* str,
                         uint32_t color) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(font);
    ASSERT_NOT_NULL(str);
    ASSERT(x < cvs->resolution.width);
    ASSERT(y < cvs->resolution.height);

    uint32_t str_len = strlen(str);
    uint32_t xx = x;
    uint32_t space_size = 1;
    uint32_t width = psf_height(font) / 2;

    for (uint32_t i = 0; i < str_len; i++) {
        canvas_draw_letter(cvs, font, xx, y, str[i], color);
        xx += space_size;
        xx += width;
    }
}

void canvas_draw_letter(struct canvas* cvs, struct psf1_font* font, uint32_t x, uint32_t y, uint8_t c, uint32_t color) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(font);
    ASSERT(x < cvs->resolution.width);
    ASSERT(y < cvs->resolution.height);

    uint32_t font_height = psf_height(font);
    uint32_t x1 = x;
    uint8_t* map = psf_character(font, c);
    for (uint8_t j = 0; j < font_height; j++) {
        uint8_t row = map[j];
        for (uint8_t i = 0; i < 8; i++) {
            if (row & 0x80) {
                canvas_draw_pixel(cvs, x1, y, color);
            }
            row = row << 1;
            x1 = x1 + 1;
        }
        y = y + 1;
        x1 = x;
    }
}

/*
* draw a bitmap
*/
void canvas_draw_bitmap(struct canvas* cvs, struct bmp* bitmap, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(cvs);
    ASSERT_NOT_NULL(bitmap);
    ASSERT(x < cvs->resolution.width);
    ASSERT(y < cvs->resolution.height);

    kprintf("offset %llu, w %llu h %llu\n", bitmap->file_header->offset, bitmap->info_header->width,
            bitmap->info_header->height);
    for (uint32_t i = 0; i < bitmap->info_header->width; i++) {
        for (uint32_t j = 0; j < bitmap->info_header->height; j++) {
            uint32_t pixel = bitmap->bitdata[(i * bitmap->info_header->width) + j];
            canvas_draw_pixel(cvs, i, j, pixel);
        }
    }
}
