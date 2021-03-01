//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NULL_OBJECT_HPP
#define _NULL_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class NullObject : public Object {
  public:
    uint64_t read();
};

#endif