//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* propertes
*/
#ifndef _PROPS_H
#define _PROPS_H

#include <types.h>

#define PROPS_KEY_SIZE 64
#define PROPS_VALUE_SIZE 64

struct arraylist;

struct prop {
    uint8_t key[PROPS_KEY_SIZE];
    uint8_t value[PROPS_VALUE_SIZE];
};

struct props {
    struct arraylist* props;
};

struct props* props_load(uint8_t* fs, uint8_t* fn);
uint8_t* props_get(struct props* props, uint8_t* name);
void props_delete(struct props* props);

#endif
