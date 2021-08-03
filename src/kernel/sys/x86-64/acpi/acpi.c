/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kprintf/kprintf.h>
#include <sys/panic/panic.h>
#include <sys/x86-64/acpi/xsdt.h>
#include <types.h>

void acpi_init() {
    rsdp_t* rsdp;

    kprintf("\tSearching for RSDP...");

    rsdp = find_rsdp_address();
    ASSERT_NOT_NULL(rsdp);

    kprintf("found at 0x%llX\n", (uint64_t)rsdp);
    return;
}