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

#define IOAPIC_REGISTER_SELECT(x, y) (*((x).IOREGSEL) = (y))

typedef enum ioapic_registers_t {
    IOAPICID = 0x0,
    IOAPICVER = 0x01,
    IOAPICARB = 0x02,
    IOREDTBL0_LO = 0x10,
    IOREDTBL0_HI = 0x11,
    IOREDTBL1_LO = 0x12,
    IOREDTBL1_HI = 0x13,
    IOREDTBL2_LO = 0x14,
    IOREDTBL2_HI = 0x15,
    IOREDTBL3_LO = 0x16,
    IOREDTBL3_HI = 0x17,
    IOREDTBL4_LO = 0x18,
    IOREDTBL4_HI = 0x19,
    IOREDTBL5_LO = 0x1A,
    IOREDTBL5_HI = 0x1B,
    IOREDTBL6_LO = 0x1C,
    IOREDTBL6_HI = 0x1D,
    IOREDTBL7_LO = 0x1E,
    IOREDTBL7_HI = 0x1F,
    IOREDTBL8_LO = 0x20,
    IOREDTBL8_HI = 0x21,
    IOREDTBL9_LO = 0x22,
    IOREDTBL9_HI = 0x23,
    IOREDTBL10_LO = 0x24,
    IOREDTBL10_HI = 0x25,
    IOREDTBL11_LO = 0x26,
    IOREDTBL11_HI = 0x27,
    IOREDTBL12_LO = 0x28,
    IOREDTBL12_HI = 0x29,
    IOREDTBL13_LO = 0x2A,
    IOREDTBL13_HI = 0x2B,
    IOREDTBL14_LO = 0x2C,
    IOREDTBL14_HI = 0x2D,
    IOREDTBL15_LO = 0x2E,
    IOREDTBL15_HI = 0x2F,
    IOREDTBL16_LO = 0x30,
    IOREDTBL16_HI = 0x31,
    IOREDTBL17_LO = 0x32,
    IOREDTBL17_HI = 0x33,
    IOREDTBL18_LO = 0x34,
    IOREDTBL18_HI = 0x35,
    IOREDTBL19_LO = 0x36,
    IOREDTBL19_HI = 0x37,
    IOREDTBL20_LO = 0x38,
    IOREDTBL20_HI = 0x39,
    IOREDTBL21_LO = 0x3A,
    IOREDTBL21_HI = 0x3B,
    IOREDTBL22_LO = 0x3C,
    IOREDTBL22_HI = 0x3D,
    IOREDTBL23_LO = 0x3E,
    IOREDTBL23_HI = 0x3F
} ioapic_registers_t;

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
uint32_t ioapic_register_read(ioapic_t ioapic, ioapic_registers_t reg);

#endif