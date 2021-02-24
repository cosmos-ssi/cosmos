//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _STR_H
#define _STR_H

#include <rootfs/lib/cosmos_types.h>

uint64_t strlen(const uint8_t* s);
uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint64_t len);

#endif