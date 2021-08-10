/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/x86-64/interrupts/apic.h>
#include <types.h>

ioapic_t** ioapic;

void ioapic_init(uint8_t acpi_id, uint32_t* base_address, uint32_t gsi_base) {
    return;
}