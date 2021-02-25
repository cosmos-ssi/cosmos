//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

uint64_t syscall(uint64_t syscall_id, void* arglist);

#endif