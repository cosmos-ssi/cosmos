//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/show_voh_command/show_voh_command.h>
#include <sys/fs/fs_facade.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>

uint8_t show_voh_function() {
    dump_VOH();
    return 1;
}

struct telnet_command* show_voh_new() {
    return telnet_command_new("show_voh", "Show VOH", &show_voh_function);
}
