//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _CONSOLE_DEVICE_HPP
#define _CONSOLE_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

class ConsoleDevice : public Device {
  public:
    uint8_t setpos(uint8_t x, uint8_t y);
    void write(const char* s);
};

#endif