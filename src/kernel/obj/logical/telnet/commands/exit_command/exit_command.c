//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/exit_command/exit_command.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/string/string.h>

uint8_t exit_function() {
    return 0;
}

struct telnet_command* exit_new() {
    return telnet_command_new("exit", "exit", &exit_function);
}
