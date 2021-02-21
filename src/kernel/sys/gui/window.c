//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/gui/window.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/video/canvas.h>

struct window* window_new(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    struct window* ret = (struct window*)kmalloc(sizeof(struct window));
    ret->height = height;
    ret->width = width;
    ret->x = x;
    ret->y = y;
    return ret;
}

void window_render(struct window* wind, struct canvas* cvs) {
    ASSERT_NOT_NULL(wind);
    ASSERT_NOT_NULL(cvs);
    kprintf("x %llu, y %llu, width %llu, height %llu\n", wind->x, wind->y, wind->width, wind->height);
    canvas_draw_line(cvs, wind->x, wind->y, wind->x + wind->width, wind->y, 0x000000);
    canvas_draw_line(cvs, wind->x, wind->y + wind->height, wind->x + wind->width, wind->y + wind->height, 0x000000);
    canvas_draw_line(cvs, wind->x, wind->y, wind->x, wind->y + wind->height, 0x000000);
    canvas_draw_line(cvs, wind->x + wind->width, wind->y, wind->x + wind->width, wind->y + wind->height, 0x000000);
    canvas_fill(cvs, wind->x + 1, wind->y + 1, wind->x + wind->width - 1, wind->y + wind->height - 1, 0x777777);
}
