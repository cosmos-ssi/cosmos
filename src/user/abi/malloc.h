//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _MALLOC_H
#define _MALLOC_H

#include <rootfs/lib/cosmos_types.h>

void* malloc(uint64_t size);
void free(void* ptr);

//void* operator new(uint64_t count);
//void operator delete(void* ptr);

#endif