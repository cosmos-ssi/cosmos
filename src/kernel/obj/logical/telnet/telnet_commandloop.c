//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/telnet_command.h>
#include <obj/logical/telnet/telnet_commandloop.h>
#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_serial.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/timing/timerapi.h>
#include <types.h>

#define TELNET_LINE_LEN 255
#define TELNET_COSMOS_PROMPT "CosmOS> "
#define TELNET_CR 13
#define TELNET_DEL 127
#define TELNET_BACKSPACE 8

void telnet_read_line(struct object* serial_object, uint8_t* line, uint16_t size) {
    ASSERT_NOT_NULL(serial_object);
    ASSERT_NOT_NULL(line);
    ASSERT_NOT_NULL(size);
    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial_object->api;
    ASSERT_NOT_NULL(serial_api);

    uint8_t read_more = 1;
    uint8_t count = 0;
    while (1 == read_more) {
        /*
        * exit..
        */
        if (count == (size - 1)) {
            line[size] = 0;
            return;
        }
        /*
        * read more
        */
        while (0 == (*serial_api->avail)(serial_object)) {
            system_sleep(100000000);
        }
        uint8_t c = (*serial_api->readchar)(serial_object);
        if (c == TELNET_CR) {
            line[count] = 0;
            return;
        } else if (c == TELNET_DEL) {
            (*serial_api->writechar)(serial_object, TELNET_BACKSPACE);
            if (count > 0) {
                count -= 1;
            }
        } else {
            //   kprintf("%llu\n", c);
            line[count] = c;
            (*serial_api->writechar)(serial_object, c);
            count += 1;
        }
    }
}

void telnet_show_help(struct arraylist* commands) {
    for (uint16_t i = 0; i < arraylist_count(commands); i++) {
        struct telnet_command* cmd = (struct telnet_command*)arraylist_get(commands, i);
        kprintf("   %s : %s\n", cmd->name, cmd->desc);
    }
    kprintf("   help : Show this help\n");
}

uint8_t telnet_process_line(uint8_t* line, struct arraylist* commands) {
    ASSERT_NOT_NULL(line);
    ASSERT_NOT_NULL(commands);
    for (uint16_t i = 0; i < arraylist_count(commands); i++) {
        struct telnet_command* cmd = (struct telnet_command*)arraylist_get(commands, i);
        if (strcmp(line, cmd->name) == 0) {
            telnet_command_function fn = cmd->command;
            return (*fn)();
        }
    }
    if (strlen(line) > 0) {
        if ((strcmp(line, "?") == 0) || (strcmp(line, "help") == 0)) {
            telnet_show_help(commands);
        } else {
            /*
        * no command was found
        */
            kprintf("Unknown command '%s' press '?' for help\n", line);
        }
    }
    return 1;
}

uint8_t telnet_command_loop(struct object* serial_object, struct arraylist* commands) {
    ASSERT_NOT_NULL(serial_object);

    struct objectinterface_serial* serial_api = (struct objectinterface_serial*)serial_object->api;
    ASSERT_NOT_NULL(serial_api);

    uint8_t go = 1;
    while (1 == go) {
        (serial_api->write)(serial_object, TELNET_COSMOS_PROMPT);
        uint8_t line[TELNET_LINE_LEN];
        memzero(line, TELNET_LINE_LEN);
        telnet_read_line(serial_object, line, TELNET_LINE_LEN);
        kprintf("\n");
        go = telnet_process_line(line, commands);
        kprintf("\n");
    }
    return 0;
}
