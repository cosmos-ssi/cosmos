/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kprintf/kprintf.h>
#include <sys/timing/timing.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>

TIMER_SOURCE_INIT(hpet, "HPET", hpet_init)

void hpet_init() {
    kprintf("hpet_init\n");
    return;
}