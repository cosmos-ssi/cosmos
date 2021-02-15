//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/cosmos_abi.h>
#include <abi/cosmos_syscall.h>

uint64_t exit() {
    return syscall_exit(0, 0);
}

uint64_t syscall_print_console(uint8_t* str) {
    return syscall(1, str);
}
