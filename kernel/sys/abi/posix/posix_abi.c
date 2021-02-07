
//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/abi/posix/posix_abi.h>
#include <sys/kprintf/kprintf.h>

uint64_t posix_abi_syscall(struct abi_syscall* syscall) {
    kprintf("posix syscall\n");

    return 0;
}
