/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _RSDT_H
#define _RSDT_H

#include <types.h>

#define RSDP_SIGNATURE "RSD PTR "

// Despite the name, the EBDA is also searched for the RSDP signature
#define EBDA_BASE 0x80000
#define EBDA_TOP 0x9FFFF

#define RSDP_SEARCH_BASE 0xE0000
#define RSDP_SEARCH_TOP 0xFFFFF

typedef void* acpi_sdt_t;

typedef enum sdt_entry_divisor_t { SDT_ENTRY_DIVISOR_RSDT = 4, SDT_ENTRY_DIVISOR_XSDT = 8 } sdt_entry_divisor_t;

typedef struct acpi_rsdp_t {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed)) acpi_rsdp_t;

typedef struct acpi_rsdp_2_t {
    acpi_rsdp_t rsdp;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed)) acpi_rsdp_2_t;

typedef struct acpi_sdt_header_t {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) acpi_sdt_header_t;

typedef struct acpi_rsdt_t {
    acpi_sdt_header_t header;
    uint32_t entries[];
} __attribute__((packed)) acpi_rsdt_t;

typedef struct acpi_xsdt_t {
    acpi_sdt_header_t header;
    uint64_t entries[];
} __attribute__((packed)) acpi_xsdt_t;

acpi_rsdp_t* acpi_find_rsdp_address();
acpi_sdt_t acpi_get_rsdt(void* rsdp);

#endif