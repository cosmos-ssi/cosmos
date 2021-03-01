
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <object/bga/bga_object.hpp>

void BGAObject::getResolution(struct objectinterface_resolution* resolution) {}
void BGAObject::setResolution(struct objectinterface_resolution* resolution) {}
uint32_t BGAObject::getBuffersize() {
    return 0;
}
void BGAObject::blt(uint8_t* buffer, uint32_t buffer_size) {}
