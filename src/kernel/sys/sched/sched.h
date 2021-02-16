/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _SCHED_H
#define _SCHED_H
#include <types.h>

// one each for each processor/core combo
extern uint64_t** current_task;

// sched_add.c
void sched_add();

// sched_init.c
void sched_init();

#endif