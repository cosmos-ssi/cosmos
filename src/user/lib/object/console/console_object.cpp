
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <abi/abi.h>
#include <assert.hpp>
}
#include <object/console/console_object.hpp>

uint8_t ConsoleObject::setPos(uint8_t x, uint8_t y) {
    return 0;
}

void ConsoleObject::write(const char* s) {
    ASSERT_NOT_NULL(s);
    syscall_console_write(this->handle, (uint8_t*)s);
}
