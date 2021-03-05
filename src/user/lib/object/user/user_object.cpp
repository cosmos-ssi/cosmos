
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <object/user/user_object.hpp>

void UserObject::get_name(struct object* obj, uint8_t* name, uint16_t size) {}
void UserObject::set_name(struct object* obj, uint8_t* name, uint16_t size) {}
void UserObject::set_pwd(struct object* obj, uint8_t* name, uint8_t* pwd, uint16_t pwd_size) {}
uint8_t UserObject::validate_pwd(struct object* obj, uint8_t* name, uint8_t* pwd) {
    return 0;
}
