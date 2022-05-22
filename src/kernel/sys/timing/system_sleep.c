/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/timing/timing.h>
#include <types.h>

uint64_t system_sleep(uint64_t ns) {
    timing_request_t* req;

    req = timing_create_request(ns);
    timer_set_alarm_relative(req);
    asm_hlt();

    return 0;
}