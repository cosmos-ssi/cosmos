/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/abi/cosmos/cosmos_abi.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/idt/exceptions.h>
#include <types.h>

void syscall_cosmos(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    struct abi_syscall syscall;
    syscall.a = 0;
    syscall.b = 0;
    syscall.c = 0;
    syscall.d = 0;
    cosmos_abi_syscall(&syscall);
}
