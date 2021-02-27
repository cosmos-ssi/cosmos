//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NIC_DEVICE_HPP
#define _NIC_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

class NICDevice : public Device {
  public:
    void read(uint8_t* data, uint16_t size);
    void write(uint8_t* data, uint16_t size);
};

#endif