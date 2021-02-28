//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_vga.h>
#include <tests/dev/test_display.h>

void test_display() {
    // get the display
    struct device* vga = devicemgr_find_device("vga0");
    if (0 != vga) {
        struct objecttype_vga* vga_api = (struct objecttype_vga*)vga->api;
        (*vga_api->write_text)(vga, "hi", 1, 1, NULL, VGA_TEXT_WHITE, VGA_TEXT_BLACK);
    } else {
        kprintf("Unable to find %s\n", vga);
    }
}
