//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _MOUSE_OBJECT_HPP
#define _MOUSE_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

struct mouse_status {
    uint8_t mouse_cycle;
    int8_t mouse_byte[3];
    int8_t mouse_x;
    int8_t mouse_y;
};

class MouseObject : public Object {
  public:
    void read(struct mouse_status* status);
};

#endif