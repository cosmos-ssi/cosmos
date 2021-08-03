//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _MEM_H
#define _MEM_H

#include <types.h>

uint8_t* memcpy(uint8_t* restrict dstptr, const uint8_t* restrict srcptr, uint64_t size);
uint8_t* memset(uint8_t* bufptr, uint8_t value, uint64_t size);
uint8_t* memzero(uint8_t* bufptr, uint64_t size);

int8_t memcmp(const void* s1, const void* s2, size_t n);

#endif