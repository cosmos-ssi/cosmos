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

typedef struct rsdp_t {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdp_address;
} __attribute__((packed)) rsdp_t;

typedef struct rsdp_2_t {
    rsdp_t rsdp;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed)) rsdp_2_t;

rsdp_t* find_rsdp_address();

#endif