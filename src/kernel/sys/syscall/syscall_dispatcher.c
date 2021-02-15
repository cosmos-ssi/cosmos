/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/syscall/syscall.h>

void syscall_dispatcher(uint64_t syscall_num, void* args) {
    kprintf("Syscall!\n");
    kprintf("num, args: %llu, 0x%llX\n", syscall_num, (uint64_t)args);

    return;
}