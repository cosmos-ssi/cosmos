//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
 * this file defines the interface that all speaker objects will implement
 */
#ifndef _OBJECTTYPE_SERIALIZER_H
#define _OBJECTTYPE_SERIALIZER_H

#include <sys/obj/objectmgr/objectmgr.h>

#include <types.h>

typedef void (*serializer_serialize_function)(struct object* ob, uint8_t* buffer, uint32_t size);
typedef void (*serializer_deserialize_function)(struct object* obj, uint8_t* buffer, uint32_t size);

struct objectinterface_serializer {
    serializer_serialize_function serialize;
    serializer_deserialize_function deserialize;
};

#endif