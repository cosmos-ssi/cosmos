//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _LOADER_H
#define _LOADER_H

#include <sys/loader/elf.h>
#include <types.h>

void loader_load(uint8_t* fs_name, uint8_t* binary_name, struct elf_binary* elf_binary);

#endif