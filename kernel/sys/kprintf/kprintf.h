/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KPRINTF_H
#define _KPRINTF_H

#include <types.h>

uint64_t kprintf(const char* s, ...);

#endif