/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic.h>
#include <sys/x86-64/interrupts/pic.h>

apic_register_t* apic_register_base;

void apic_init() {
    // Initialize PIC so we can remap IRQs, then mask all interrupts
    pic_init();
    pic_disable();

    apic_register_base = acpi_get_local_apic_address();

    ASSERT_NOT_NULL(apic_register_base);

    return;
}