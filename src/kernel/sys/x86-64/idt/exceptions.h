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

void isrBoundRangeExceeded(stack_frame* frame);
void isrDE(stack_frame* frame);
void isrDeviceNotAvailable(stack_frame* frame);
void isrDoubleFault(stack_frame_error* frame);
void isrPFE(stack_frame_error* frame);
void isrGeneric(stack_frame* frame);
void isrGPF(stack_frame_error* frame);
void isrInvalidOpcode(stack_frame* frame);
void isrNMI(stack_frame* frame);
void isrOverflow(stack_frame* frame);
void isrDebug(stack_frame* frame);
void isrBreakpoint(stack_frame* frame);

#endif