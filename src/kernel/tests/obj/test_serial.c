//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/debug/debug.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <tests/obj/test_serial.h>

/*
 * write a message via the uniform serial api
 */
void serialMessage(const uint8_t* message) {
    // get serial0
    struct object* serial0 = objectmgr_find_object("serial0");
    if (0 != serial0) {
        struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial0->api;
        serial_write_function write_func = serial_api->write;
        (*write_func)(serial0, message);
    } else {
        kprintf("Unable to find serial0\n");
    }
}

void test_serial() {
    /*
     * exercise the uniform serial API
     */
    serialMessage(
        "This message brought to you by the uniform serial API, the letters R and S and the Digits 2, 3 and 2\n");
}
