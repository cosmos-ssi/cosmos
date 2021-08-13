/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

ioapic_t* ioapic;
uint64_t num_ioapic;

void ioapic_init(acpi_madt_record_ioapic_t** madt_ioapic) {
    acpi_madt_record_interrupt_source_override_t** madt_iso;
    uint64_t i = 0;
    uint64_t j = 0;

    while (madt_ioapic[i]) {
        kprintf("\tIOAPIC %hu, ACPI ID %hu, base address 0x%llX, GSI base %lu\n", i, madt_ioapic[i]->id,
                (uint64_t)madt_ioapic[i]->address, madt_ioapic[i]->gsi_base);
        i++;
    }

    ioapic = (ioapic_t*)kmalloc(sizeof(ioapic_t) * i);

    for (j = 0; j < i; j++) {
        ioapic[j].acpi_id = madt_ioapic[j]->id;
        ioapic[j].irq_low = madt_ioapic[j]->gsi_base;
        ioapic[j].IOREGSEL = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address);
        ioapic[j].IOREGWIN = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address + 0x10);

        /* 
         * The value stored in the register is # of input pins - 1.  This is the
         * correct value, without adjustment, to add to irq_low to specify the
         * full range.  For example, if irq_low is 0, and the register value is
         * 23 (meaning 24 input pins), then we add 23 to irq_low to get an
         * irq_high of 23, and 0-23 is 24 distinct values.
         */

        ioapic[j].irq_high = IOAPICVER_EXTRACT_MAXREDIR(ioapic_register_read(ioapic[j], IOAPICVER)) + ioapic[j].irq_low;
    }

    madt_iso = acpi_enumerate_interrupt_source_override();

    i = 0;
    while (madt_iso[i]) {
        kprintf("\tIRQ %hu is remapped to GSI %lu with flags %u\n", madt_iso[i]->source, madt_iso[i]->gsi,
                madt_iso[i]->flags);
        i++;
    }

    return;
}

uint32_t ioapic_register_read(ioapic_t ioapic, ioapic_registers_t reg) {
    IOAPIC_REGISTER_SELECT(ioapic, reg);

    return *(ioapic.IOREGWIN);
}

void ioapic_register_write(ioapic_t ioapic, ioapic_registers_t reg, uint32_t val) {
    IOAPIC_REGISTER_SELECT(ioapic, reg);

    *(ioapic.IOREGWIN) = val;

    return;
}