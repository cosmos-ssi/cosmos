//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <init.h>

// use this abi to print a char to screen
#include <abi/cosmos/cosmos_abi.h>

/*
* we are in user-land context here.  This is the entry point to userland that the kernel code calls
*/
void cosmos_userland_init() {
    cosmos_abi_com1_write('^');
    cosmos_abi_com1_write('H');
    cosmos_abi_com1_write('I');
    cosmos_abi_com1_write('^');
    cosmos_abi_com1_write('\n');

    // here we mount a disk, I presume, and start reading files.
    // we likely end up starting a console for the console user to log into
}
