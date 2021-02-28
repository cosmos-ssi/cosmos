//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/console/serial_console.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_console.h>

void test_serial_console_dev() {
    uint8_t devicename[] = {"serial0"};

    /*
     * find the physical disk
     */
    struct object* serial = objectmgr_find_object(devicename);
    if (0 != serial) {
        // attach the console
        struct object* console_device = serial_console_attach(serial);

        struct objecttype_console* console_api = (struct objecttype_console*)console_device->api;

        (*console_api->write)(console_device, "Console test\n");

        // detach the console
        serial_console_detach(console_device);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
