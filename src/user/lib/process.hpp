//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
/*
* userland wrapper for kernel process object
*/
#ifndef _PROCESS_HPP
#define _PROCESS_HPP

#include <heap.hpp>
#include <types.h>

class Process {
  private:
    Heap* heap;

  public:
    Process();
    void sleep(uint64_t ms);
    uint64_t pid();
    Process* fork();
};

#endif