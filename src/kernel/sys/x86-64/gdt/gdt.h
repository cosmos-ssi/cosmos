//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Getting_to_Ring_3

#ifndef _GDT_H
#define _GDT_H

#include <types.h>

#define GDT_KERNEL_CODE_SELECTOR 8
#define GDT_KERNEL_DATA_SELECTOR 16
#define GDT_USER_CODE_SELECTOR 24
#define GDT_USER_DATA_SELECTOR 32

void gdt_install();
void extern gdt_flush();

#endif