//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_floppy.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <tests/obj/test_floppy.h>
#include <types.h>

void floppyread() {
    // get the floppy
    struct object* floppy = objectmgr_find_object("floppy0");
    if (0 != floppy) {
        struct objectinterface_floppy* floppy_api = (struct objectinterface_floppy*)floppy->api;

        uint8_t data[256];
        (*floppy_api->read)(floppy, 0, data, 255);
        debug_show_memblock(data, 32);
    } else {
        kprintf("Unable to find floppy0\n");
    }
}
