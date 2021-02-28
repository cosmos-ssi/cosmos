/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/misc.h>
#include <sys/debug/assert.h>
#include <sys/x86-64/idt/irq.h>
#include <sys/x86-64/mm/mm.h>

void isrPFE_handler(stack_frame_error* frame) {
    ASSERT_NOT_NULL(frame);

    page_fault_handler(frame->error, asm_cr2_read(), asm_cr3_read());

    return;
}
