//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <init.hpp>

#include <device/device.hpp>
#include <device/filesystem/filesystem_device.hpp>
#include <device/serial/serial_device.hpp>

#define INIT_SERIAL_DEVICE "serial0"
#define INIT_ROOT_FS_DEVICE "fs0"

/*
* we are in user-land context here.  This is the entry point to userland that the kernel code calls
*/
void init() {
    //   SerialDevice* serialDevice = (SerialDevice*)Device::find(INIT_SERIAL_DEVICE);
    //  if (0 != serialDevice) {
    //      serialDevice->writeln("Hello from user land");
    //  }

    //  FilesystemDevice* filesystemDevice = (FilesystemDevice*)Device::find(INIT_ROOT_FS_DEVICE);
    //   if (0 != filesystemDevice) {
    //       serialDevice->writeln("Found root file system");
    //   }
    // guess we're done...
    //  syscall_exit();
    // here we mount a disk, I presume, and start reading files.
    // we likely end up starting a console for the console user to log into
}
