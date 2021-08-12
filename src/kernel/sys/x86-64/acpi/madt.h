/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _MADT_H
#define _MADT_H

#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/interrupts/apic_register.h>
#include <types.h>

// per https://uefi.org/specs/ACPI/6.4/05_ACPI_Software_Programming_Model/ACPI_Software_Programming_Model.html#i-o-apic-structure
typedef enum __attribute__((packed)) acpi_madt_record_types_t {
    ACPI_MADT_RECORD_PROCESSOR_LAPIC = 0,  // be careful that you don't confuse this with LSAPIC below
    ACPI_MADT_RECORD_IOAPIC =
        1,  // be careful especially if using an editor with auto-complete, this is very similar to IOSAPIC below
    ACPI_MADT_RECORD_INTERRUPT_SOURCE_OVERRIDE = 2,
    ACPI_MADT_RECORD_NMI_SOURCE = 3,
    ACPI_MADT_RECORD_LAPIC_NMI = 4,
    ACPI_MADT_RECORD_LAPIC_ADDRESS_OVERRIDE = 5,
    ACPI_MADT_RECORD_IOSAPIC =
        6,  // be careful especially if using an editor with auto-complete, this is very similar to IOAPIC above
    ACPI_MADT_RECORD_PROCESSOR_LSAPIC = 7,  // be careful that you don't confuse this with LAPIC above
    ACPI_MADT_RECORD_PLATFORM_INTERRUPT_SOURCE = 8,
    ACPI_MADT_RECORD_PROCESSOR_LX2APIC = 9,
    ACPI_MADT_RECORD_PROCESSOR_LX2APIC_NMI = 10,
    ACPI_MADT_RECORD_GICC = 11,
    ACPI_MADT_RECORD_GICD = 12,
    ACPI_MADT_RECORD_GIC_MSI_FRAME = 13,
    ACPI_MADT_RECORD_GICR = 14,
    ACPI_MADT_RECORD_GIC_ITS = 15,
    ACPI_MADT_RECORD_MULTIPROCESSOR_WAKEUP = 16
} acpi_madt_record_types_t;

typedef struct acpi_madt_record_prologue_t {
    acpi_madt_record_types_t type;
    uint8_t length;
} __attribute__((packed)) acpi_madt_record_prologue_t;

typedef struct acpi_madt_record_ioapic_t {
    uint8_t id;
    uint8_t reserved;
    uint32_t address;  // physical address
    uint32_t gsi_base;
} __attribute__((packed)) acpi_madt_record_ioapic_t;

typedef struct acpi_madt_record_interrupt_source_override_t {
    uint8_t bus;
    uint8_t source;
    uint32_t gsi;
    uint16_t flags;
} __attribute__((packed)) acpi_madt_record_interrupt_source_override_t;

typedef struct acpi_madt_t {
    acpi_sdt_header_t header;
    uint32_t local_apic_address;
    uint32_t flags;
} __attribute__((packed)) acpi_madt_t;

acpi_madt_record_interrupt_source_override_t** acpi_enumerate_interrupt_source_override();
acpi_madt_record_ioapic_t** acpi_enumerate_ioapic();
apic_register_t* acpi_get_local_apic_address();

#endif