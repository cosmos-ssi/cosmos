//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TICK_OBJECT_HPP
#define _TICK_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class TickObject : public Object {
  public:
    uint64_t read();
};

#endif