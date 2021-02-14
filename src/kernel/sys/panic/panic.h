/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _PANIC_H
#define _PANIC_H

#include <types.h>

void panic(const char* s, const char* fn, uint32_t line);

#define PANIC(message) panic(#message, __FILE__, __LINE__);

#endif