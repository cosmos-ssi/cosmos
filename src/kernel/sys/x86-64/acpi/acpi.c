/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

const char* acpi_table_signatures[] = {"RSD PTR ", "RSDT", "XSDT", "APIC"};

acpi_sdt_t rsdt;

#define RSDT_PTR(x) ((acpi_rsdt_t*)(x))
#define XSDT_PTR(x) ((acpi_xsdt_t*)(x))

void* acpi_find_table(acpi_table_types_t type) {
    sdt_entry_divisor_t divisor;
    uint64_t entries;
    uint64_t i;
    acpi_sdt_header_t* tmp;

    ASSERT_NOT_NULL(rsdt);

    // since memcmp returns 0 if equal, then if the rsdt header signature is RSDT it'll return 0
    if (!memcmp(RSDT_PTR(rsdt)->header.signature, acpi_table_signatures[ACPI_RSDT],
                strlen(acpi_table_signatures[ACPI_RSDT]))) {
        divisor = SDT_ENTRY_DIVISOR_RSDT;
    } else {  // then it's an XSDT
        divisor = SDT_ENTRY_DIVISOR_XSDT;
    }

    // Whether it's an RSDT or an XSDT, the header is the same, so we cast it to
    // an RSDT so we can get the signature

    entries = (RSDT_PTR(rsdt)->header.length - sizeof(RSDT_PTR(rsdt)->header)) / divisor;

    for (i = 0; i < entries; i++) {
        switch (divisor) {
            case SDT_ENTRY_DIVISOR_RSDT:
                tmp = CONV_PHYS_ADDR((acpi_sdt_header_t*)(uint64_t)(RSDT_PTR(rsdt)->entries[i]));
                break;
            case SDT_ENTRY_DIVISOR_XSDT:
                tmp = CONV_PHYS_ADDR((acpi_sdt_header_t*)XSDT_PTR(rsdt)->entries[i]);
                break;
            default:  // can't happen
                panic("Invalid SDT divisor", __FILE__, __LINE__);
                break;
        }

        if (!memcmp(tmp->signature, acpi_table_signatures[type], strlen(acpi_table_signatures[type]))) {
            return tmp;
        }
    }

    return 0;  // table not found
}

void acpi_init() {
    acpi_rsdp_t* rsdp;
    acpi_rsdp_2_t* rsdp_2 = 0;

    rsdt = 0;

    kprintf("\tSearching for RSDP...");

    rsdp = acpi_find_rsdp_address();
    ASSERT_NOT_NULL(rsdp);

    kprintf("found at 0x%llX\n", (uint64_t)rsdp);

    // TODO: Checksum verification

    // 0 = ACPI 1.0, points to RSDT; 2 = ACPI >= 2.0, points to XSDT
    if (rsdp->revision == 2) {
        kprintf("\tThis is an ACPI 2.0+ RSDP\n");
        rsdp_2 = (acpi_rsdp_2_t*)rsdp;

        // Farther down we print a diagnostic message with the address of the
        // root SDT; we go ahead and print the initial "X" (for XSDT) or "R"
        // (for RSDT) here, as appropriate, so we don't have to do another
        // comparison later on.
        kprintf("\tX");
    } else {
        kprintf("\tThis is an ACPI 1.0 RSDP\n");
        kprintf("\tR");
    }

#define RSDP (rsdp_2 ? (void*)rsdp_2 : (void*)rsdp)

    rsdt = CONV_PHYS_ADDR(acpi_get_rsdt(RSDP));

    kprintf("SDT at 0x%llX physical\n", CONV_DMAP_ADDR(rsdt));

#undef RSDP

    return;
}