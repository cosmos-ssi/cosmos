/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/string/mem.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/mm/pagetables.h>

acpi_rsdp_t* acpi_find_rsdp_address() {
    /* Returns PHYSICAL address of RSDP struct--NOT address of RSDT/XSDT.  Read the RSDP
     * struct to find that.
     */

    uint64_t i;

    // RSDP can be found in one of two areas--either the first KB of the
    // extended BIOS data area or the region from 0xE0000 to 0xFFFFF.  We have
    // to search both for the "RSD PTR " signature.  It is always located on a
    // sixteen-byte boundary.

    for (i = EBDA_BASE; i < (EBDA_BASE + 1024); i += 16) {
        // We have to use memcmp and not str(n)cmp because the signature is not null-terminated
        if (!memcmp(CONV_PHYS_ADDR((void*)i), acpi_table_signatures[ACPI_RSDP], 8)) {
            return (acpi_rsdp_t*)i;
        }
    }

    for (i = RSDP_SEARCH_BASE; i < RSDP_SEARCH_TOP; i += 16) {
        if (!memcmp(CONV_PHYS_ADDR((void*)i), acpi_table_signatures[ACPI_RSDP], 8)) {
            return (acpi_rsdp_t*)i;
        }
    }

    return 0;  // RSDP not found
}

acpi_sdt_t acpi_get_rsdt(void* rsdp) {

    if (((acpi_rsdp_t*)rsdp)->revision == 2) {
        // cast to a rsdp_2, return xsdt_address cast to a pointer to a sdt
        return (acpi_sdt_t)((acpi_rsdp_2_t*)rsdp)->xsdt_address;
    } else {
        // cast to a rsdp, access 32-bit rsdt_address, cast rsdt_address to a
        // 64-bit integer, then cast this to a pointer to a sdt
        return (acpi_sdt_t)((uint64_t)((acpi_rsdp_t*)rsdp)->rsdt_address);
    }
}