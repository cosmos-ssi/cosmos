/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _IRQ_H
#define _IRQ_H

#include <types.h>

typedef struct stack_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t fflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) stack_frame;

void irq0_handler(stack_frame* frame);
void irq1_handler(stack_frame* frame);
void irq2_handler(stack_frame* frame);
void irq3_handler(stack_frame* frame);
void irq4_handler(stack_frame* frame);
void irq5_handler(stack_frame* frame);
void irq6_handler(stack_frame* frame);
void irq7_handler(stack_frame* frame);
void irq8_handler(stack_frame* frame);
void irq9_handler(stack_frame* frame);
void irq10_handler(stack_frame* frame);
void irq11_handler(stack_frame* frame);
void irq12_handler(stack_frame* frame);
void irq13_handler(stack_frame* frame);
void irq14_handler(stack_frame* frame);
void irq15_handler(stack_frame* frame);

#endif
