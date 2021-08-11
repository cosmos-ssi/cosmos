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
        ioapic[j].gsi_base = madt_ioapic[j]->gsi_base;
        ioapic[j].IOREGSEL = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address);
        ioapic[j].IOREGWIN = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address + 0x10);

        *(ioapic[j].IOREGSEL) = 0x01;
        kprintf("0x01 IOAPIC register: 0x%lX\n", *(ioapic[j].IOREGWIN));
    }

    return;
}