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

#define IOAPICVER_EXTRACT_MAXREDIR(x) (((x)&0xFF0000) >> 16)

#define IOAPIC_REDIR_BASE_VEC 0x20

#define LEGACY_IRQ_MAX 0xF

// macros to construct redirection table entries
// This first group operates on the low-order dword
#define IOAPIC_REDTBL_INTVEC_SET(entry, vec) ((entry) |= (uint32_t)(vec))        // Set interrupt vector
#define IOAPIC_REDTBL_DELMOD_SET(entry, mode) ((entry) |= (mode << 8))           // Set delivery mode
#define IOAPIC_REDTBL_DESTMOD_SET(entry, mode) ((entry) |= (mode << 11))         // Set destination mode
#define IOAPIC_REDTBL_INTPOL_SET(entry, polarity) ((entry) |= (polarity << 13))  // Set polarity
#define IOAPIC_REDTBL_TRIGMODE_SET(entry, mode) ((entry) |= (mode << 15))        // Set trigger mode
#define IOAPIC_REDTBL_INTMASK_SET(entry, mask) ((entry) |= (mask << 16))         // Set interrupt mask

// This one operates on the high-order dword
#define IOAPIC_REDTBL_DEST_SET(entry, dest) ((entry) |= (dest << 24))  // Destination CPU/group

typedef enum ioapic_redtbl_delmod_t {
    IOREDTBL_DELMOD_FIXED = 0b000,
    IOREDTBL_DELMOD_LOWPRI = 0b001,
    IOREDTBL_DELMOD_SMI = 0b010,
    IOREDTBL_DELMOD_NMI = 0b100,
    IOREDTBL_DELMOD_INIT = 0b101,
    IOREDTBL_DELMOD_EXTINT = 0b111
} ioapic_redtbl_delmod_t;

typedef enum ioapic_redtbl_destmod_t {
    IOREDTBL_DESTMOD_PHYSICAL = 0,
    IOREDTBL_DESTMOD_LOGICAL = 1
} ioapic_redtbl_destmod_t;

typedef enum ioapic_redtbl_intmask_t {
    IOREDTBL_INTMASK_ACTIVE = 0,
    IOREDTBL_INTMASK_MASKED = 1
} ioapic_redtbl_intmask_t;

typedef enum ioapic_redtbl_intpol_t { IOREDTBL_INTPOL_HIGH = 0, IOREDTBL_INTPOL_LOW = 1 } ioapic_redtbl_intpol_t;

typedef enum ioapic_redtbl_trigmode_t {
    IOREDTBL_TRIGMODE_EDGE = 0,
    IOREDTBL_TRIGMODE_LEVEL = 1
} ioapic_redtbl_trigmode_t;

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
    uint32_t max_redir;
} ioapic_t;

void apic_init();
void apic_send_eoi();

void ioapic_init(acpi_madt_record_ioapic_t** madt_ioapic);
uint32_t ioapic_register_read(ioapic_t ioapic, ioapic_registers_t reg);
void ioapic_register_write(ioapic_t ioapic, ioapic_registers_t reg, uint32_t val);

#endif