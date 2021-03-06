//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _GROUP_OBJECT_HPP
#define _GROUP_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class GroupObject : public Object {
  public:
    void get_name(struct object* obj, uint8_t* name, uint16_t size);
};

#endif