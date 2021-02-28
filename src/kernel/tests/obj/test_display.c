//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_vga.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_display.h>
#include <types.h>

void test_display() {
    // get the display
    struct object* vga = objectmgr_find_object("vga0");
    if (0 != vga) {
        struct objectinterface_vga* vga_api = (struct objectinterface_vga*)vga->api;
        (*vga_api->write_text)(vga, "hi", 1, 1, NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);
    } else {
        kprintf("Unable to find %s\n", vga);
    }
}
