//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/cosmos/cosmos_abi.h>

extern void cosmos_syscall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

uint64_t cosmos_abi_com1_write(uint8_t c) {
    cosmos_syscall(c, 0, 0, 0);
    uint64_t ret = 0;
    //  uint64_t interrupt_number = 0x80;
    //  asm volatile("push %0" : "r"(c));
    //  asm volatile("int $interrupt_number");
    return ret;
}
