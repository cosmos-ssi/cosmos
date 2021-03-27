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
#include <object/serial/serial_object.hpp>

void SerialObject::writeln(const char* str) {
    ASSERT_NOT_NULL(str);
}

uint8_t SerialObject::readChar() {
    return 0;
}
void SerialObject::writeChar(uint8_t c) {
    syscall_serial_writechar(this->handle, c);
}

void SerialObject::writestring(const uint8_t* str) {
    ASSERT_NOT_NULL(str);

    uint16_t i = 0;
    uint8_t c = str[i];
    while (c != 0) {
        writeChar(c);
        i += 1;
        c = str[i];
    }
}