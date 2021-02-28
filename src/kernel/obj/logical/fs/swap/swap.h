//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SWAP_H
#define _SWAP_H

struct object;

struct object* swap_attach(struct object* block_device);
void swap_detach(struct object* dev);

#endif
