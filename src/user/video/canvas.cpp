//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <malloc.h>
}
#include <assert.hpp>
#include <bmp.hpp>
#include <canvas.hpp>
#include <psf.hpp>
#include <rgb.hpp>
#include <tga.hpp>

canvas::canvas(BGAObject* bga) {
    ASSERT_NOT_NULL(bga);
    this->bga = bga;
    /*
    * useful state data
    */
    this->buffer_size = bga->getBuffersize();
    // kprintf("Canvas buffer size %#llX\n", ret->buffer_size);
    bga->getResolution(&(this->resolution));
    this->buffer = (uint8_t*)malloc(this->buffer_size);
    this->bytes_per_pixel = this->resolution.color_depth / 8;
}

canvas::~canvas() {
    ASSERT_NOT_NULL(this->buffer);
    free(this->buffer);
}

uint32_t canvas::pixel_offset(uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(this->resolution.width);
    ASSERT_NOT_NULL(this->resolution.height);
    ASSERT(x <= this->resolution.width);
    ASSERT(y <= this->resolution.height);
    ASSERT(this->bytes_per_pixel > 0);
    return this->bytes_per_pixel * ((this->resolution.width * y) + x);
}

void canvas::dump() {

    //   kprintf("width %llu, height %llu depth %llu size %llu\n", this->resolution.width, this->resolution.height,
    //         this->resolution.color_depth, this->buffer_size);
}

void canvas::blt() {
    this->bga->blt(this->buffer, this->buffer_size);
}

void canvas::clear(uint32_t rgb) {
    /*
    * components
    */
    rgb_components* components = new rgb_components(rgb);
    //  kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, components.r, components.g, components.b);
    /*
    * paint
    */
    for (uint32_t i = 0; i < (this->resolution.width * this->resolution.height * this->bytes_per_pixel);
         i += this->bytes_per_pixel) {
        this->buffer[i] = components->b;
        this->buffer[i + 1] = components->g;
        this->buffer[i + 2] = components->r;
    }
    delete components;
}

void canvas::draw_pixel(uint32_t x, uint32_t y, uint32_t rgb) {
    //   kprintf("x %#llX, y%#llX, w %#llX, h %#llX\n", x, y, this->resolution.width, this->resolution.height);

    ASSERT(x <= this->resolution.width);
    ASSERT(y <= this->resolution.height);

    uint32_t offset = pixel_offset(x, y);
    //  kprintf("offset %#llX,\n", offset);
    rgb_components* components = new rgb_components(rgb);

    //  kprintf("rgb %#llX, r %#llX, g %#llX, b %#llX\n", rgb, components.r, components.g, components.b);

    this->buffer[offset] = components->b;
    this->buffer[offset + 1] = components->g;
    this->buffer[offset + 2] = components->r;

    delete components;
}

uint32_t canvas::abs_diff(uint32_t x1, uint32_t x2) {
    if (x2 > x1) {
        return x2 - x1;
    }
    return x1 - x2;
}

/*
* https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
*/
void canvas::draw_sloped_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    ASSERT(x0 < this->resolution.width);
    ASSERT(y0 < this->resolution.height);
    ASSERT(x1 < this->resolution.width);
    ASSERT(y1 < this->resolution.height);

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
    draw_pixel(x, y, rgb);

    while (x < xEnd) {
        x++;
        if (p < 0)
            p += twoDy;
        else {
            y++;
            p += twoDyDx;
        }
        draw_pixel(x, y, rgb);
    }
}

void canvas::draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    ASSERT(x0 < this->resolution.width);
    ASSERT(y0 < this->resolution.height);
    ASSERT(x1 < this->resolution.width);
    ASSERT(y1 < this->resolution.height);

    //    kprintf("x0 %llu, y0 %llu, x1 %llu, y1 %llu\n", x0, y0, x1, y1);

    if (x0 == x1) {
        if (y1 > y0) {
            for (uint32_t i = y0; i < y1; i++) {
                draw_pixel(x0, i, rgb);
            }
        } else {
            for (uint32_t i = y1; i < y0; i++) {
                draw_pixel(x0, i, rgb);
            }
        }
    } else if (y0 == y1) {
        if (x1 > x0) {
            for (uint32_t i = x0; i < x1; i++) {
                draw_pixel(i, y0, rgb);
            }
        } else {
            for (uint32_t i = x1; i < x0; i++) {
                draw_pixel(i, y0, rgb);
            }
        }
    } else {
        draw_sloped_line(x0, y0, x1, y1, rgb);
    }
}

void canvas::fill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t rgb) {
    ASSERT(x0 < this->resolution.width);
    ASSERT(y0 < this->resolution.height);
    ASSERT(x1 < this->resolution.width);
    ASSERT(y1 < this->resolution.height);

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
            draw_pixel(i, j, rgb);
        }
    }
}

void canvas::draw_letters(psf1_font* font, uint32_t x, uint32_t y, uint8_t* str, uint32_t color) {
    ASSERT_NOT_NULL(font);
    ASSERT_NOT_NULL(str);
    ASSERT(x < this->resolution.width);
    ASSERT(y < this->resolution.height);

    uint32_t str_len = String::strlen(str);
    uint32_t xx = x;
    uint32_t space_size = 1;
    uint32_t width = font->height() / 2;

    for (uint32_t i = 0; i < str_len; i++) {
        draw_letter(font, xx, y, str[i], color);
        xx += space_size;
        xx += width;
    }
}

void canvas::draw_letter(psf1_font* font, uint32_t x, uint32_t y, uint8_t c, uint32_t color) {
    ASSERT_NOT_NULL(font);
    ASSERT(x < this->resolution.width);
    ASSERT(y < this->resolution.height);

    uint32_t font_height = font->height();
    uint32_t x1 = x;
    uint8_t* map = font->character(c);
    for (uint8_t j = 0; j < font_height; j++) {
        uint8_t row = map[j];
        for (uint8_t i = 0; i < 8; i++) {
            if (row & 0x80) {
                draw_pixel(x1, y, color);
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
void canvas::draw_bitmap(bmp* bitmap, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(bitmap);
    ASSERT(x <= this->resolution.width);
    ASSERT(y <= this->resolution.height);

    //   kprintf("offset %llu, w %llu h %llu\n", bitmap->file_header->offset, bitmap->info_header->width,
    //           bitmap->info_header->height);
    for (uint32_t i = 0; i < bitmap->info_header->width; i++) {
        for (uint32_t j = 0; j < bitmap->info_header->height; j++) {
            uint32_t pixel = bitmap->bitdata[(i * bitmap->info_header->width) + j];
            draw_pixel(i, j, pixel);
        }
    }
}

/*
* draw a targa
*/
void canvas::draw_targa(tga* targa, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(targa);
    ASSERT(x <= this->resolution.width);
    ASSERT(y <= this->resolution.height);
    //  tga_dump(targa);

    for (uint32_t i = 0; i < targa->header->w; i++) {
        for (uint32_t j = 0; j < targa->header->h; j++) {
            uint32_t pixel = targa->data[(i * targa->header->w) + j];
            draw_pixel(i, j, pixel);
        }
    }
}
