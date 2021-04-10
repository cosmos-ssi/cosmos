//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <init.hpp>

#include <cosmos.hpp>

#define INIT_SERIAL_DEVICE "serial0"
#define INIT_ROOT_FS_DEVICE "fs0"
#define INIT_TERM_PROGRAM "term"

/*
* we are in user-land context here.  This is the entry point to userland that the kernel code calls
*/
uint64_t main(uint64_t argc, uint8_t* argv[]) {
    SerialObject* serialDevice = (SerialObject*)Object::find(INIT_SERIAL_DEVICE);
    if (0 != serialDevice) {
        serialDevice->writeln("Hello from user land");
    }

    FilesystemObject* filesystemDevice = (FilesystemObject*)Object::find(INIT_ROOT_FS_DEVICE);
    if (0 != filesystemDevice) {
        serialDevice->writeln("Found root file system");
    }

    //    struct fs_node* root_node = filesystemDevice->getRootNode();
    //    struct fs_node*  term_node = filesystemDevice->

    return 0;
}
