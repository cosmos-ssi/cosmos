/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <types.h>

void acpi_init() {
    rsdp_t* rsdp;
    rsdp_2_t* rsdp_2 = 0;

    kprintf("\tSearching for RSDP...");

    rsdp = find_rsdp_address();
    ASSERT_NOT_NULL(rsdp);

    kprintf("found at 0x%llX\n", (uint64_t)rsdp);

    // 0 = ACPI 1.0, points to RSDT; 2 = ACPI >= 2.0, points to XSDT
    if (rsdp->revision == 2) {
        kprintf("\tThis is an ACPI 2.0+ RSDP\n");
        rsdp_2 = (rsdp_2_t*)rsdp;
    } else {
        kprintf("\tThis is an ACPI 1.0 RSDT\n");
    }

    return;
}