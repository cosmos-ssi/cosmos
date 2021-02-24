//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <init.h>

// use this abi to print a char to screen
#include <abi/abi.h>

/*
* we are in user-land context here.  This is the entry point to userland that the kernel code calls
*/
void cosmos_userland_init() {
    syscall_print_console('^');
    syscall_print_console('H');
    syscall_print_console('I');
    syscall_print_console('^');
    syscall_print_console('\n');

    // guess we're done...
    //  syscall_exit();
    // here we mount a disk, I presume, and start reading files.
    // we likely end up starting a console for the console user to log into
}
