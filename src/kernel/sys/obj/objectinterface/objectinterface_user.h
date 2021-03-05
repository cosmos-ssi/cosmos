//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECTTYPE_USER_H
#define _OBJECTTYPE_USER_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*user_get_name_function)(struct object* obj, uint8_t* name, uint16_t size);

struct objectinterface_user {
    user_get_name_function name;
};

#endif