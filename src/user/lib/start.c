// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Creating_a_C_Library

#include <start.h>

// "main" exported by the userland application
extern int main(int argc, char* argv[]);

// this is the entry point
/*
*
*/
int _start() {
    int ret = main(0, 0);
    // call exit() syscall here
    return ret;
}