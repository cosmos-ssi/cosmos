//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIAL_OBJECT_HPP
#define _SERIAL_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class SerialObject : public Object {
  public:
    void writeln(const char* str);
    uint8_t readChar();
    void writeChar(uint8_t c);
    void writestring(const uint8_t* str);
};

#endif