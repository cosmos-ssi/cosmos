//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <memory.hpp>
#include <process.hpp>

#define PROCESS_DEFAULT_MEMORY 1024 * 100  // 100k. I am a generous person.

Process::Process() {
    void* memory = Memory::malloc(PROCESS_DEFAULT_MEMORY);
    this->heap = new Heap();
    this->heap->add((uint64_t)memory, PROCESS_DEFAULT_MEMORY, 1024);
}

void Process::sleep(uint64_t ms) {}

uint64_t Process::pid() {
    return 0;
}

Process* Process::fork() {
    return 0;
}
