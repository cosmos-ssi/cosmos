//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include <string.hpp>

class Object {
  protected:
    String name;
    uint64_t handle;

  public:
    static Object* find(String name);
    static Object* find(const char* name);
    static Object* find(uint64_t handle);
};

#endif