//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _HEAP_HPP
#define _HEAP_HPP

// https://wiki.osdev.org/User:Pancakes/BitmapHeapImplementation

#include <types.h>

struct Heap_block_bitmap {
    struct Heap_block_bitmap* next;
    uint32_t size;
    uint32_t used;
    uint32_t bsize;
    uint32_t lfb;
};

class Heap {
  public:
    Heap();
    int add(uint64_t addr, uint32_t size, uint32_t bsize);
    void* alloc(uint32_t size);
    void free(void* ptr);

  private:
    uint8_t getNID(uint8_t a, uint8_t b);
    Heap_block_bitmap* fblock;
};

#endif