//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _LINKAGE_H
#define _LINKAGE_H

#include <types.h>

uint64_t linkage_get_kernel_size();
void linkage_show_kernel_section_data();
uint64_t linkage_get_kernel_sector_count();
#endif