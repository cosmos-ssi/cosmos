//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BGA_DEVICE_HPP
#define _BGA_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

class BGADevice : public Device {
  public:
    void getResolution(struct objectinterface_resolution* resolution);
    void setResolution(struct objectinterface_resolution* resolution);
    uint32_t getBuffersize();
    void blt(uint8_t* buffer, uint32_t buffer_size);
};

#endif