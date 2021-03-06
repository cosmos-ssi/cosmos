//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _USER_OBJECT_HPP
#define _USER_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class UserObject : public Object {
  public:
    void get_name(struct object* obj, uint8_t* name, uint16_t size);
    void set_name(struct object* obj, uint8_t* name, uint16_t size);
    void set_pwd(struct object* obj, uint8_t* name, uint8_t* pwd, uint16_t pwd_size);
    uint8_t validate_pwd(struct object* obj, uint8_t* name, uint8_t* pwd);
};

#endif