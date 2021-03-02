//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/telnet_commandloop.h>
#include <sys/debug/assert.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <types.h>

uint8_t telnet_command_loop(struct object* serial_object) {
    ASSERT_NOT_NULL(serial_object);

    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial_object->api;
    ASSERT_NOT_NULL(serial_api);

    uint8_t go = 1;
    while (1 == go) {
        (serial_api->write)(serial_object, "> ");

        // read line
        // process line

        // temporary
        go = 0;
    }
    return 0;
}
