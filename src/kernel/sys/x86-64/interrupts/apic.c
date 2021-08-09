/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic.h>
#include <sys/x86-64/interrupts/pic.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

apic_register_t* apic_register_base;

void apic_init() {
    uint32_t siv;

    // Initialize PIC so we can remap IRQs, then mask all interrupts
    pic_init();
    pic_disable();

    apic_register_base = CONV_PHYS_ADDR(acpi_get_local_apic_address());
    kprintf("\tAPIC Register base: 0x%llX\n", (uint64_t)apic_register_base);
    ASSERT_NOT_NULL(apic_register_base);

    // Read spurious interrupt vector register, then set bit 8 for software
    // LAPIC enable and set the low-order byte to 0xFF to use that as the
    // spurious interrupt vector
    siv = APIC_REGISTER(0xF);
    siv |= 0x1FF;
    APIC_REGISTER(0xF) = siv;

    return;
}