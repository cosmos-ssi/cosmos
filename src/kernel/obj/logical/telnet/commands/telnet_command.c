//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/telnet_command.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

struct telnet_command* telnet_command_new(uint8_t* command, uint8_t* desc, telnet_command_function command_function) {
    ASSERT_NOT_NULL(command);
    ASSERT_NOT_NULL(desc);
    ASSERT_NOT_NULL(command_function);
    ASSERT(strlen(command) < TELNET_COMMAND_NAME_LEN);
    struct telnet_command* ret = (struct telnet_command*)kmalloc(sizeof(struct telnet_command));
    strncpy(ret->name, command, TELNET_COMMAND_NAME_LEN);
    strncpy(ret->desc, desc, TELNET_DESC_NAME_LEN);
    ret->command = command_function;
    return ret;
}
