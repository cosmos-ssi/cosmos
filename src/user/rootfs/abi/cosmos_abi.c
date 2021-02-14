//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <rootfs/abi/cosmos_abi.h>
#include <rootfs/abi/cosmos_syscall.h>

uint64_t syscall_print_console() {
    return syscall(1);
}
