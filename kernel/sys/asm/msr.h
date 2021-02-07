/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MSR_H
#define _MSR_H

uint64_t asm_rdmsr(uint64_t reg);
void asm_wrmsr(uint64_t reg, uint64_t val);

#endif