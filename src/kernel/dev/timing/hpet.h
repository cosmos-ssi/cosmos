/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _HPET_H
#define _HPET_H

#include <subsystems.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <types.h>

typedef struct acpi_hpet_t {
    acpi_sdt_header_t header;
    uint32_t etb_id;
    acpi_gas_t address;
    uint8_t hpet_num;
    uint16_t min_clock_tick;
    uint8_t page_protection;
} __attribute__((packed)) acpi_hpet_t;

typedef struct hpet_timer_registers_t {
    uint64_t configuration_capability;
    uint64_t comparator_value;
    uint64_t fsb_interrupt_route;
    uint64_t reserved;
} __attribute__((packed)) hpet_timer_registers_t;

typedef struct hpet_general_capabilities_id_register_t {
    uint8_t revision;
    uint8_t flags;
    uint16_t vendor_id;
    uint32_t period;  // in femtoseconds (10 ^ -15)
} __attribute__((packed)) hpet_general_capabilities_id_register_t;

typedef struct hpet_main_registers_t {
    // It is volatile--in that it can conceivably be changed by the system,
    // independent of the code--and it is const in that we should not be
    // modifying it ourselves.
    const volatile hpet_general_capabilities_id_register_t general_capabilities_id;  // 0x000-0x007
    uint64_t reserved1;                                                              // 0x008-0x00F
    uint64_t general_configuration;                                                  // 0x010-0x017
    uint64_t reserved2;                                                              // 0x018-0x01F
    uint64_t general_interrupt_status;                                               // 0x020-0x027
    uint64_t reserved3[25];                                                          // 0x028-0x0EF
    uint64_t main_counter_value;                                                     // 0x0F0-0x0F7
    uint64_t reserved4;                                                              // 0x0F8-0x0FF
    hpet_timer_registers_t timer_registers[];
} __attribute__((packed)) hpet_main_registers_t;

// use these macros with hpet_main_registers_t.general_interrupt_status
#define HPET_MAIN_ENABLE(x) ((x) |= 1)
#define HPET_MAIN_DISABLE(x) ((x) &= 0xFFFFFFFFFFFFFFFE)
#define HPET_LEGACY_ENABLE(x) ((x) |= (1 << 1));
#define HPET_LEGACY_DISABLE(x) ((x) &= 0xFFFFFFFFFFFFFFFD)

void hpet_init(driver_list_entry_t* driver_list_entry);

#endif