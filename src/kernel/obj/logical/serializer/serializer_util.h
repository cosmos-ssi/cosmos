//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIALIZER_UTIL_H
#define _SERIALIZER_UTIL_H

#include <types.h>

struct dynabuffer;
struct object;

void serialize(struct object* serializer_object, struct object* obj, struct dynabuffer* buffer);
void deserialize(struct object* serializer_object, struct object* obj, struct dynabuffer* buffer);

#endif