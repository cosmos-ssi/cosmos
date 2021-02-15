/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kprintf/kprintf.h>
#include <sys/syscall/syscall.h>

uint64_t sys_exit(void* args) {
    kprintf("sys_exit, argument %llX\n", (uint64_t)args);

    return 0;
}