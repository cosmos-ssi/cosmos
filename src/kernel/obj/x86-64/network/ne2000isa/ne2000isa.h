//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _NE2000ISA_H
#define _NE2000ISA_H

#include <types.h>

void ne2000isa_objectmgr_register_objects();

void ne2000isa_send(uint8_t* packet, uint16_t length);
uint16_t ne2000isa_recieve(uint8_t* packet, uint16_t max_size);

#endif