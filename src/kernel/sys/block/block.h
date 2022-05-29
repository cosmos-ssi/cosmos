/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SYS_BLOCK_H
#define _SYS_BLOCK_H

#include <subsystems.h>

void block_init(driver_list_entry_t** drivers);

#endif