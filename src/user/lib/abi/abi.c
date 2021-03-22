//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <abi/syscall.h>

uint64_t syscall_process_exit() {
    return syscall0(SYSCALL_PROCESS_EXIT);
}

uint64_t syscall_process_sleep(uint64_t time) {
    return syscall1(SYSCALL_PROCESS_SLEEP, (void*)time);
}

uint64_t syscall_console_write(uint8_t* str) {
    return syscall1(SYSCALL_CONSOLE_WRITE, str);
}

uint64_t syscall_memory_malloc(uint64_t size) {
    return syscall1(SYSCALL_MEMORY_MALLOC, (void*)size);
}

uint64_t syscall_memory_free(void* mem) {
    return syscall1(SYSCALL_MEMORY_FREE, (void*)mem);
}

uint64_t syscall_memory_realloc(void* mem, uint64_t size) {
    return syscall2(SYSCALL_MEMORY_REALLOC, (void*)mem, (void*)size);
}
