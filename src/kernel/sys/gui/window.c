//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/assert.h>
#include <sys/gui/gui.h>
#include <sys/gui/window.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>
#include <sys/video/canvas.h>

struct window* window_new(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    struct window* ret = (struct window*)kmalloc(sizeof(struct window));
    ret->titlebar_height = 20;
    window_set_position(ret, x, y);
    window_set_dimensions(ret, width, height);
    ret->background_color = 0x777777;
    ret->title_background_color = 0x052433;  // dark blue
    ret->border_color = 0x052433;            // dark blue
    ret->title_font_color = 0xDDDDDD;        // white-ish
    strncpy(ret->title, "CosmOS", WINDOW_MAX_TITLE);
    return ret;
}

void window_render(struct window* wind, struct canvas* cvs) {
    ASSERT_NOT_NULL(wind);
    ASSERT_NOT_NULL(cvs);
    //  kprintf("x %llu, y %llu, width %llu, height %llu\n", wind->x, wind->y, wind->width, wind->height);
    canvas_draw_line(cvs, wind->x, wind->y, wind->x + wind->width, wind->y, wind->border_color);
    canvas_draw_line(cvs, wind->x, wind->y + wind->titlebar_height, wind->x + wind->width,
                     wind->y + wind->titlebar_height, wind->border_color);
    canvas_draw_line(cvs, wind->x, wind->y + wind->height, wind->x + wind->width, wind->y + wind->height,
                     wind->border_color);
    canvas_draw_line(cvs, wind->x, wind->y, wind->x, wind->y + wind->height, wind->border_color);
    canvas_draw_line(cvs, wind->x + wind->width, wind->y, wind->x + wind->width, wind->y + wind->height,
                     wind->border_color);
    canvas_fill(cvs, wind->x + 1, wind->y + wind->titlebar_height + 1, wind->x + wind->width - 1,
                wind->y + wind->height - 1, wind->background_color);
    canvas_fill(cvs, wind->x + 1, wind->y + 1, wind->x + wind->width - 1, wind->y + wind->titlebar_height - 1,
                wind->title_background_color);
    canvas_draw_letters(cvs, gui_state->font, wind->x + 2, wind->y + 2, wind->title, wind->title_font_color);
}

void window_set_position(struct window* wind, uint32_t x, uint32_t y) {
    ASSERT_NOT_NULL(wind);
    wind->x = x;
    wind->y = y;
}

void window_set_dimensions(struct window* wind, uint32_t width, uint32_t height) {
    ASSERT_NOT_NULL(wind);
    wind->height = height;
    if (wind->height < wind->titlebar_height) {
        wind->height = wind->titlebar_height;
    }
    wind->width = width;
}
