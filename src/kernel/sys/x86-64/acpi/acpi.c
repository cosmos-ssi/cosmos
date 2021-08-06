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
    acpi_rsdp_t* rsdp;
    acpi_rsdp_2_t* rsdp_2 = 0;
    acpi_sdt_t rsdt;
    sdt_entry_divisor_t divisor;

    kprintf("\tSearching for RSDP...");

    rsdp = acpi_find_rsdp_address();
    ASSERT_NOT_NULL(rsdp);

    kprintf("found at 0x%llX\n", (uint64_t)rsdp);

    // TODO: Checksum verification

    // 0 = ACPI 1.0, points to RSDT; 2 = ACPI >= 2.0, points to XSDT
    if (rsdp->revision == 2) {
        kprintf("\tThis is an ACPI 2.0+ RSDP\n");
        rsdp_2 = (acpi_rsdp_2_t*)rsdp;

        // This is what the value of the length field in the SDT is divided by
        // to get the number of indices into the entries field.
        divisor = SDT_ENTRY_DIVISOR_XSDT;

        // Farther down we print a diagnostic message with the address of the
        // root SDT; we go ahead and print the initial "X" (for XSDT) or "R"
        // (for RSDT) here, as appropriate, so we don't have to do another
        // comparison later on.
        kprintf("\tX");
    } else {
        kprintf("\tThis is an ACPI 1.0 RSDP\n");
        divisor = SDT_ENTRY_DIVISOR_RSDT;
        kprintf("\tR");
    }

#define RSDP (rsdp_2 ? (void*)rsdp_2 : (void*)rsdp)

    rsdt = acpi_get_rsdt(RSDP);

    kprintf("SDT at 0x%llX\n", rsdt);

#undef RSDP

    return;
}