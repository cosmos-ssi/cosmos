//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _GUI_H
#define _GUI_H

#include <types.h>

struct gui_state_data {
    uint32_t width;
    uint32_t height;
    uint32_t color_depth;
    uint32_t buffer_size;
    uint8_t* buffer;
};

void gui_init();

#endif
