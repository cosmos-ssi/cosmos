
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/bga/bga_device.hpp>

void BGADevice::getResolution(struct objectinterface_resolution* resolution) {}
void BGADevice::setResolution(struct objectinterface_resolution* resolution) {}
uint32_t BGADevice::getBuffersize() {
    return 0;
}
void BGADevice::blt(uint8_t* buffer, uint32_t buffer_size) {}
