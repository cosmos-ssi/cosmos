/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/madt.h>

void* acpi_get_local_apic_address() {
    // this returns the PHYSICAL address
    acpi_madt_t* madt;

    madt = (acpi_madt_t*)acpi_find_table(ACPI_MADT);

    return (void*)(uint64_t)(madt->local_apic_address);
}