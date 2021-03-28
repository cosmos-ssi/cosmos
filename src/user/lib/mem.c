//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <mem.h>

uint8_t* memcpy(uint8_t* dstptr, const uint8_t* srcptr, uint64_t size) {
    for (uint64_t i = 0; i < size; i++)
        dstptr[i] = srcptr[i];
    return dstptr;
}

uint8_t* memset(uint8_t* bufptr, uint8_t value, uint64_t size) {
    for (uint64_t i = 0; i < size; i++)
        bufptr[i] = (unsigned char)value;
    return bufptr;
}

uint8_t* memzero(uint8_t* bufptr, uint64_t size) {
    return memset(bufptr, 0, size);
}
