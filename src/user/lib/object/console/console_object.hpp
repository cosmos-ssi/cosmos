//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CONSOLE_OBJECT_HPP
#define _CONSOLE_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class ConsoleObject : public Object {
  public:
    uint8_t setPos(uint8_t x, uint8_t y);
    void write(const char* s);
};

#endif