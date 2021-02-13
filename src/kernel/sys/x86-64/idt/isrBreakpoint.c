/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Tom Everett                           *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/idt/exceptions.h>
#include <types.h>

void isrBreakpoint_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("Breakpoint Exception\n");
}
