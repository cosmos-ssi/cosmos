//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef __ABI_H
#define __ABI_H

#include <types.h>

/*
* architecture-independent syscall structure
*/
struct abi_syscall {
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
};

#endif