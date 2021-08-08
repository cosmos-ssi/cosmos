/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/x86-64/acpi/madt.h>

void* apic_register_base;

void apic_init() {
    apic_register_base = acpi_get_local_apic_address();

    ASSERT_NOT_NULL(apic_register_base);

    return;
}