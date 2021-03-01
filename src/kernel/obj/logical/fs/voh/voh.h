//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _VOH_H
#define _VOH_H

#include <types.h>

struct filesystem_node;
struct object;

struct object* voh_attach(uint8_t* name);
void voh_detach(struct object* obj);

void voh_add_child(struct object* voh_device, struct filesystem_node* child_node);
void voh_remove_child(struct object* voh_device, uint64_t id);

#endif