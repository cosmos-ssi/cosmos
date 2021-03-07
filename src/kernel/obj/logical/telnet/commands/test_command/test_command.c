//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/logical/telnet/commands/test_command/test_command.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/string/string.h>
#include <tests/tests.h>

uint8_t test_function() {
    tests_run();
    return 1;
}

struct telnet_command* test_new() {
    return telnet_command_new("test", "run tests", &test_function);
}
