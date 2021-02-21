//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _GUI_H
#define _GUI_H

#include <types.h>

struct canvas;

struct gui_state_data {
    struct canvas* canvas;
};

void gui_init();

#endif
