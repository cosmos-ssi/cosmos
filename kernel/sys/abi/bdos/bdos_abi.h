
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BDOS_ABI_H
#define _BDOS_ABI_H

#include <sys/abi/abi.h>

uint64_t bdos_abi_syscall(struct abi_syscall* syscall);

#endif
