//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _ACPI_H
#define _ACPI_H

#include <types.h>

// https://wiki.osdev.org/RSDT

struct acpi_sdt_header {
    uint8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t oem_id[6];
    uint8_t oem_table[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct rsdt {
    struct acpi_sdt_header h;
    uint32_t* pointerToOtherSDT;
};

struct xsdt {
    struct acpi_sdt_header h;
    uint64_t* pointerToOtherSDT;
};

void acpi_objectmgr_register_objects();

struct fadt* acpi_get_fadt();
#endif