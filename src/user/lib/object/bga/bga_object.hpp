//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BGA_OBJECT_HPP
#define _BGA_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class BGAObject : public Object {
  public:
    void getResolution(struct syscall_objectinterface_resolution* resolution);
    void setResolution(struct syscall_objectinterface_resolution* resolution);
    uint32_t getBuffersize();
    void blt(uint8_t* buffer, uint32_t buffer_size);
};

#endif