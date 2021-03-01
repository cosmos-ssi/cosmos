//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NIC_OBJECT_HPP
#define _NIC_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class NICObject : public Object {
  public:
    void read(uint8_t* data, uint16_t size);
    void write(uint8_t* data, uint16_t size);
};

#endif