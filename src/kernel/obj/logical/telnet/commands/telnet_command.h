//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _TELNET_COMMAND_H
#define _TELNET_COMMAND_H

#include <types.h>

#define TELNET_COMMAND_NAME_LEN 128
#define TELNET_DESC_NAME_LEN 128

typedef uint8_t (*telnet_command_function)();

struct telnet_command {
    uint8_t name[TELNET_COMMAND_NAME_LEN];
    uint8_t desc[TELNET_DESC_NAME_LEN];
    telnet_command_function command;
};

struct telnet_command* telnet_command_new(uint8_t* command, uint8_t* desc, telnet_command_function command_function);

#endif
