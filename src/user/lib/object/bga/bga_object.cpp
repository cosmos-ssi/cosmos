
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <abi/abi.h>
#include <assert.h>
}

#include <object/bga/bga_object.hpp>

void BGAObject::getResolution(struct syscall_objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(resolution);
    syscall_bga_get_resolution(this->handle, resolution);
}

void BGAObject::setResolution(struct syscall_objectinterface_resolution* resolution) {
    ASSERT_NOT_NULL(resolution);
    syscall_bga_set_resolution(this->handle, resolution);
}

uint32_t BGAObject::getBuffersize() {
    return syscall_bga_get_buffersize(this->handle);
}

void BGAObject::blt(uint8_t* buffer, uint32_t buffer_size) {
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(buffer_size);
    syscall_bga_blt(this->handle, buffer, buffer_size);
}
