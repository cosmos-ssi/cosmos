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

void isrGPF_handler(stack_frame_error* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("GPF!\n");
    kprintf("Error code: 0x%llX\n", frame->error);
    kprintf("Return address: 0x%llX\n", frame->rip);
    kprintf("CS: %llX\n", frame->cs);
    kprintf("RFLAGS: %llX\n", frame->rflags);
    kprintf("RSP: %llX\n", frame->rsp);
    kprintf("SS: %llX\n", frame->ss);
    PANIC("GPF Exception\n");
}
