/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt Weber                                 *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/panic/panic.h>
#include <sys/x86-64/idt/irq.h>

void isrBoundRangeExceeded_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    PANIC("Bound range exceeded!\n");
}
