/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/timing/timing.h>
#include <types.h>

uint64_t system_sleep(uint64_t ns) {
    timer_set_alarm_relative(ns);

    return 0;
}