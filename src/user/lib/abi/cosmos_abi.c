//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/cosmos_abi.h>
#include <abi/cosmos_syscall.h>

uint64_t exit() {
    //   return syscall_exit(0, 0);
    return 0;
}

uint64_t syscall_print_console(uint8_t* str) {
    return syscall(1, str);
}

uint64_t syscall_malloc(uint64_t size) {
    return syscall(2, (void*)size);
}

uint64_t syscall_free(void* mem) {
    return syscall(3, mem);
}

uint64_t syscall_realloc(void* mem, uint64_t size) {
    //  return syscall(4, mem, size);
    return 0;
}

uint64_t syscall_sleep(uint64_t time) {
    return syscall(5, (void*)time);
}