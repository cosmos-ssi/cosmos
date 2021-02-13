//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <rootfs/abi/cosmos_abi.h>

#define COSMOS_SYSCALL_COM1_WRITE 0x01

extern void cosmos_syscall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

uint64_t cosmos_abi_com1_write(uint8_t c) {
    cosmos_syscall(COSMOS_SYSCALL_COM1_WRITE, c, 0, 0);
    return 0;
}
