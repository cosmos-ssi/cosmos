//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <lib/types.h>

void syscall(uint64_t syscall, void* param);
void tge();

#endif
