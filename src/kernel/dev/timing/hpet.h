/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _HPET_H
#define _HPET_H

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

#endif