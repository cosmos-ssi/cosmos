
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _DYNABUFFER_H
#define _DYNABUFFER_H

#include <types.h>

struct dynabuffer {
    uint8_t* data;
    uint32_t size;
};

struct dynabuffer* dynabuffer_new();
void dynabuffer_delete(struct dynabuffer* db);

#endif
