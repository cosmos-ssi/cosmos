//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NODE_UTIL_H
#define _NODE_UTIL_H

#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <types.h>

struct object;

struct filesystem_node* filesystem_node_new(enum filesystem_node_type type, struct object*, const uint8_t* name,
                                            uint64_t size, void* node_data, uint64_t parent);

#endif