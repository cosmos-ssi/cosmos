//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* a userland object that represents kernel memory.  This is object is how we allocate blocks of user memory.
*/
#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include <types.h>

class Memory {

  public:
    static void free(void* ptr);
    static void* malloc(uint64_t size);
    static void* realloc(void* ptr, uint64_t size);
};

#endif