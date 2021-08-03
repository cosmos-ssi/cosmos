//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Meaty_Skeleton

#include <sys/debug/assert.h>
#include <sys/string/mem.h>
#include <types.h>

int8_t memcmp(const void* s1, const void* s2, size_t n) {
    ASSERT_NOT_NULL(s1);
    ASSERT_NOT_NULL(s2);
    ASSERT_NOT_NULL(n);

    uint64_t i;

    /* 
       Return 0 if first n elements at s1 are equal to first n elements at s2.

       Return -1 if the first element of s1 that differs from its corresponding
       element of s2, is less than the element in s2; return 1 if it is greater
       than.
    */

    for (i = 0; i < n; i++) {
        if (((BYTE*)s1)[i] == ((BYTE*)s2)[i]) {
            // If they're equivalent then continue to next element.
            continue;
        }

        /*
           If they're unequal, then determine how they're unequal and return the
           proper value.  One inequality means the strings are not equal, so we
           can return immediately.
        */

        return ((((BYTE*)s1)[i] > ((BYTE*)s2)[i]) ? 1 : -1);
    }

    /* 
       If we reach this point then there have been no elements that differ, so
       return 0 for equal
    */
    return 0;
}

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
