//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include <types.h>

struct object* serializer_attach();
void serializer_detach(struct object* obj);

#endif