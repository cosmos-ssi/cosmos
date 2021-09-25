/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _TIMERAPI_H
#define _TIMERAPI_H

#include <types.h>

// Returns number of seconds slept for; takes time to sleep in nanoseconds
// Only for sleeping within the kernel itself--not for userspace
uint64_t system_sleep(uint64_t ns);

#endif