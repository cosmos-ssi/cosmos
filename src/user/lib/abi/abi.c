//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <abi/abi.h>
#include <abi/syscall.h>

uint64_t syscall_process_exit() {
    return 0;
}

uint64_t syscall_process_sleep(uint64_t time) {
    return 0;
}

uint64_t syscall_console_write(uint8_t* str) {
    return 0;
}

uint64_t syscall_memory_malloc(uint64_t size) {
    return 0;
}

uint64_t syscall_memory_free(void* mem) {
    return 0;
}

uint64_t syscall_memory_realloc(void* mem, uint64_t size) {
    return 0;
}
