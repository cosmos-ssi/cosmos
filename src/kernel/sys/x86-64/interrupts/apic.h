/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _APIC_H
#define _APIC_H

#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic_register.h>
#include <types.h>

// APIC registers are 32-bits wide, but they're spaced 16 bytes (128 bits)
// apart.  This will take care of calculating the proper offset for us.
#define APIC_REGISTER(x) (apic_register_base[(x)*4])

typedef struct ioapic_t {
    uint32_t* IOREGSEL;
    uint32_t* IOREGWIN;
    uint8_t acpi_id;
    uint32_t irq_low;
    uint32_t irq_high;
} ioapic_t;

extern ioapic_t* ioapic;
extern uint64_t num_ioapic;

void apic_init();
void ioapic_init(acpi_madt_record_ioapic_t** madt_ioapic);

#endif