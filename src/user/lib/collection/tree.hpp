//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TREE_HPP
#define _TREE_HPP

#include <types.h>

template <typename T> class Tree {
  private:
    T* value;
    uint64_t key;
    Tree* left;
    Tree* right;

  public:
    Tree();
    ~Tree();
    void insert(uint64_t key, T* value);
    T* search(uint64_t key);
};

#endif
