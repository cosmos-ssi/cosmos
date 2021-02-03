//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Meaty_Skeleton

#include <sys/debug/assert.h>
#include <sys/string/mem.h>

uint8_t* memcpy(uint8_t* restrict dstptr, const uint8_t* restrict srcptr, uint64_t size) {
    ASSERT_NOT_NULL(dstptr);
    ASSERT_NOT_NULL(srcptr);
    ASSERT(size > 0);

    for (uint64_t i = 0; i < size; i++)
        dstptr[i] = srcptr[i];
    return dstptr;
}

uint8_t* memset(uint8_t* bufptr, uint8_t value, uint64_t size) {
    ASSERT_NOT_NULL(bufptr);
    ASSERT(size > 0);

    for (uint64_t i = 0; i < size; i++)
        bufptr[i] = (unsigned char)value;
    return bufptr;
}

uint8_t* memzero(uint8_t* bufptr, uint64_t size) {
    return memset(bufptr, 0, size);
}
