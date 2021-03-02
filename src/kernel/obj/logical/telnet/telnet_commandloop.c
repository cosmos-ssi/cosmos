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
#include <sys/string/mem.h>
#include <types.h>

#define TELNET_LINE_LEN 255

void telnet_read_line(struct object* serial_object, uint8_t* line, uint16_t size) {
    ASSERT_NOT_NULL(serial_object);
    ASSERT_NOT_NULL(line);
    ASSERT_NOT_NULL(size);
    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial_object->api;
    ASSERT_NOT_NULL(serial_api);

    uint8_t read_more = 1;
    uint8_t count = 0;
    while (1 == read_more) {
        //    uint8_t c = (*serial_api->)
        /*
        * exit..
        */
        if (count == (size - 1)) {
            line[size] = 0;
            return;
        }
        count += 1;
    }
}

uint8_t telnet_command_loop(struct object* serial_object) {
    ASSERT_NOT_NULL(serial_object);

    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial_object->api;
    ASSERT_NOT_NULL(serial_api);

    uint8_t go = 1;
    while (1 == go) {
        (serial_api->write)(serial_object, "> ");
        uint8_t line[TELNET_LINE_LEN];
        memzero(line, TELNET_LINE_LEN);
        telnet_read_line(serial_object, line, TELNET_LINE_LEN);
        // process line

        // temporary
        go = 0;
    }
    return 0;
}
