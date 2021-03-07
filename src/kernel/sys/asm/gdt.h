/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _GDT_H
#define _GDT_H

#include <types.h>

void asm_lgdt(void* gdt);
uint8_t* asm_sgdt();

#endif