//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _HOSTID_OBJECT_HPP
#define _HOSTID_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class HostIdObject : public Object {
  public:
    uint64_t get_id();
};

#endif