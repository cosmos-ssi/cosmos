/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _DE_H
#define _DE_H

#include <sys/debug/assert.h>
#include <sys/x86-64/idt/irq.h>
#include <types.h>

void isrDE(stack_frame* frame);
void isrPFE(stack_frame* frame, uint64_t error);
void isrGeneric(stack_frame* frame);
void isrGPF(stack_frame* frame);
void isrDebug(stack_frame* frame);
void isrBreakpoint(stack_frame* frame);

#endif