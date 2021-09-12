/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet.h>
#include <subsystems.h>
#include <sys/kprintf/kprintf.h>
#include <sys/timing/timing.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/mm/pagetables.h>

SUBSYSTEM_DRIVER(hpet, "High-Performance Event Timer", "High-Performance Event Timer", "Kurt M. Weber",
                 "weberk294@gmail.com", "0.1", 0xb7a2e515a3f4c2a3, 0x5b47216f5833f1de, 0x879c22ee8cf15ff5,
                 0x7e23d05814a1cfb5, SUBSYSTEM_ID_TIMER, &hpet_init);

void hpet_init(driver_list_entry_t* driver_list_entry) {
    acpi_hpet_t* acpi_hpet;
    driver_info_1_t* di;
    hpet_main_registers_t* hpet_registers;

    di = (driver_info_1_t*)(driver_list_entry->driver_info);

    kprintf("\t%s: %s driver (%s) version %s loaded.  Contact %s <%s>.\n", di->shortname, di->longname, di->description,
            di->version, di->authors, di->contact);

    acpi_hpet = acpi_find_table(ACPI_HPET);
    kprintf("\tHPET ACPI table at 0x%llX\n", (uint64_t)acpi_hpet);

    hpet_registers = (hpet_main_registers_t*)CONV_PHYS_ADDR(acpi_hpet->address.address);
    kprintf("\tHPET register base at 0x%llX\n", hpet_registers);

    kprintf("\tRevision, flags, ID, period: %hu, %hu, %u, %lu\n", hpet_registers->general_capabilities_id.revision,
            hpet_registers->general_capabilities_id.flags, hpet_registers->general_capabilities_id.vendor_id,
            hpet_registers->general_capabilities_id.period);

    HPET_LEGACY_ENABLE(hpet_registers->general_configuration);
    HPET_MAIN_ENABLE(hpet_registers->general_configuration);
    kprintf("\tLegacy/main enable: 0x%llX\n", hpet_registers->general_configuration);

    return;
}