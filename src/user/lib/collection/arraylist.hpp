//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ARRAYLIST_HPP
#define _ARRAYLIST_HPP

#include <collection/array.hpp>
#include <types.h>

template <typename T> class ArrayList {
  private:
    class Array<T> arr;
    uint32_t count;

  public:
    ArrayList();
    ~ArrayList();
    uint32_t getCount();
    uint32_t getSize();
    uint32_t add(T* value);
    void set(uint32_t position, T* value);
    T* get(uint32_t position);
    void remove(uint32_t position);
};

#endif