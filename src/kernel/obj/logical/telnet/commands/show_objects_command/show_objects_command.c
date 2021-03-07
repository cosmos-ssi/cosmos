//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/show_objects_command/show_objects_command.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/string/string.h>

uint8_t show_objects_function() {
    objectmgr_dump_objects();
    return 1;
}

struct telnet_command* show_objects_new() {
    return telnet_command_new("show_objects", "Show Objects", &show_objects_function);
}
