//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIAL_DEVICE_HPP
#define _SERIAL_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

class SerialDevice : public Device {
  public:
    void writeln(const char* str);
};

#endif