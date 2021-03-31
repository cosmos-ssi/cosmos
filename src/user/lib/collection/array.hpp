//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARRAY_HPP
#define _ARRAY_HPP

#include <types.h>

template <typename T> class Array {
  private:
    T** data;
    uint32_t size;
    uint32_t byte_size;

  public:
    Array(uint32_t size);
    ~Array();
    void set(uint32_t position, T* value);
    T* get(uint32_t position);
    uint32_t getSize();
    void resize(uint32_t size);
    void grow(uint32_t increment);
};

#endif