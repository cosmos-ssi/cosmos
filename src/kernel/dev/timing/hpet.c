/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet.h>
#include <subsystems.h>
#include <sys/kprintf/kprintf.h>
#include <sys/timing/timing.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>

SUBSYSTEM_DRIVER(hpet, "High-Performance Event Timer", "High-Performance Event Timer", "Kurt M. Weber",
                 "weberk294@gmail.com", "0.1", 0xb7a2e515a3f4c2a3, 0x5b47216f5833f1de, 0x879c22ee8cf15ff5,
                 0x7e23d05814a1cfb5, SUBSYSTEM_ID_TIMER, &hpet_init);

void hpet_init(void* driver_info) {
    kprintf("hpet_init\n");
    return;
}