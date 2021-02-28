//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/console/vga_console.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/object.h>
#include <sys/objectmgr/objectmgr.h>
#include <sys/objecttype/objecttype_console.h>
#include <types.h>

void test_vga_console_dev() {
    uint8_t devicename[] = {"vga0"};

    /*
     * find the physical disk
     */
    struct object* vga = objectmgr_find_object(devicename);
    if (0 != vga) {
        // attach the console
        struct object* console_device = vga_console_attach(vga);

        struct objecttype_console* console_api = (struct objecttype_console*)console_device->api;

        (*console_api->write)(console_device, "Console test\n");

        // detach the console
        vga_console_detach(console_device);
    } else {
        kprintf("Unable to find %s\n", devicename);
    }
}
