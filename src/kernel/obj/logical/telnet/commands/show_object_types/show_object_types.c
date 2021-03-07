//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/show_object_types/show_object_types.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/string/string.h>

uint8_t show_object_types_function() {
    objecttypes_dump();
    return 1;
}

struct telnet_command* show_object_types_new() {
    return telnet_command_new("show_object_types", "Show Object Types", &show_object_types_function);
}
