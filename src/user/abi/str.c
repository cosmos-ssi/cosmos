//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/assert.h>
#include <abi/str.h>

uint64_t strlen(const uint8_t* s) {
    ASSERT_NOT_NULL(s);
    uint64_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint64_t len) {
    ASSERT_NOT_NULL(dest);
    ASSERT_NOT_NULL(src);
    ASSERT(len > 0);
    uint64_t i;
    for (i = 0; ((i < len - 1) && (src[i] != 0)); i++) {
        dest[i] = src[i];
    }
    dest[i] = 0;
    return dest;
}
