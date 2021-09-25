/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet.h>
#include <subsystems.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/timing/timing.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

SUBSYSTEM_DRIVER(hpet, "High-Precision Event Timer", "High-Precision Event Timer", "Kurt M. Weber",
                 "weberk294@gmail.com", "0.1", 0xb7a2e515a3f4c2a3, 0x5b47216f5833f1de, 0x879c22ee8cf15ff5,
                 0x7e23d05814a1cfb5, SUBSYSTEM_ID_TIMER, &hpet_init);

const uint64_t hpet_max_period = 0x05F5E100;

void hpet_interrupt_irq_0();
void hpet_interrupt_irq_8();

uint64_t hpet_calc_frequency(hpet_main_registers_t* hpet_registers) {
    uint64_t scaled_max_period;
    uint64_t frequency_units;
    uint64_t descale_value;

    /* This might be a bit complicated.  A few things to note first:
     * 
     * 1) The HPET does not directly tell us a frequency; rather, it tells us
     * a period, in units of femtoseconds (10^-15 seconds).
     * 
     * 2) It's trivial in theory to convert a period to a frequency, except
     * we're constrained by (a) the need to stick to integer calculations, and
     * (b) the need to fit everything within a 64-bit unsigned integer,
     * including all values for/from intermediate calculations.
     * 
     * 3) The largest permissible value for the HPET period is 0x05F5E100.
     * This is 100,000,000 decimal, and a period of 100,000,000 femtoseconds
     * is equal to a period of 100 nanoseconds, which corresponds to a
     * frequency of 10 MHz.  Thus, the minimum permissible frequency is 10 MHz.
     */

    // First, we scale the HPET maximum period to minimize the fractional part
    // that will be lost due to truncation in subsequent integer divisions. 100
    // billion is the largest integer power of ten that we can multiply the
    // maximum period by and fit within a 64-bit unsigned integer.
    scaled_max_period = hpet_max_period * HPET_PERIOD_SCALE_VALUE;

    // Then, we divide it by the period the HPET's configuration register tells
    // us.  This gives us the frequency in 10 MHz units (after accounting for
    // the previous scaling operation), so e.g. a value of 2 means it operates
    // at 20 MHz, 10 means it runs at 100 MHz, etc.
    frequency_units = scaled_max_period / hpet_registers->general_capabilities_id.period;

    // So that when we descale frequency_units we'll get units of Hz, rather than 10 MHz
    descale_value = HPET_PERIOD_SCALE_VALUE / HPET_MIN_FREQ;

    return frequency_units / descale_value;
}

void* hpet_init(driver_list_entry_t* driver_list_entry, void* timing_driver) {
    acpi_hpet_t* acpi_hpet;
    driver_info_1_t* di;
    hpet_main_registers_t* hpet_registers;
    timing_driver_t* td;
    timing_source_t* sources;
    uint64_t i;

    td = (timing_driver_t*)timing_driver;
    di = (driver_info_1_t*)(driver_list_entry->driver_info);

    kprintf("\t%s: %s driver (%s) version %s loaded.  Contact %s <%s>.\n", di->shortname, di->longname, di->description,
            di->version, di->authors, di->contact);

    acpi_hpet = acpi_find_table(ACPI_HPET);

    hpet_registers = (hpet_main_registers_t*)CONV_PHYS_ADDR(acpi_hpet->address.address);
    kprintf("\tHPET register base at 0x%llX\n", hpet_registers);

    kprintf("\tRevision, flags, ID, period: %hu, %hu, %u, %lu\n", hpet_registers->general_capabilities_id.revision,
            hpet_registers->general_capabilities_id.flags, hpet_registers->general_capabilities_id.vendor_id,
            hpet_registers->general_capabilities_id.period);

    kprintf("Frequency: %llu Hz\n", hpet_calc_frequency(hpet_registers));

    td->calibrate = NULL;
    td->driver_id[0] = di->id[0];
    td->driver_id[1] = di->id[1];
    td->driver_id[2] = di->id[2];
    td->driver_id[3] = di->id[3];
    td->num_sources = 2;
    td->type = TIMING_SOURCE_HPET;

    sources = kmalloc(3 * sizeof(timing_source_t));
    for (i = 0; i < 3; i++) {
        sources[i].type = TIMING_SOURCE_HPET;
        sources[i].driver_id[0] = di->id[0];
        sources[i].driver_id[1] = di->id[1];
        sources[i].driver_id[2] = di->id[2];
        sources[i].driver_id[3] = di->id[3];
        sources[i].driver_source_idx = i;
        sources[i].calibrated = true;
        sources[i].frequency = hpet_calc_frequency(hpet_registers);
    }

    //HPET_MAIN_ENABLE(hpet_registers->general_configuration);

    //HPET_LEGACY_ENABLE(hpet_registers->general_configuration);

    //interrupt_router_register_interrupt_handler(0, hpet_interrupt_irq_0);
    //interrupt_router_register_interrupt_handler(8, hpet_interrupt_irq_8);

    return (void*)sources;
}

void hpet_interrupt_irq_0() {
    //kprintf("PIT HPET\n");
    return;
}

void hpet_interrupt_irq_8() {
    //kprintf("RTC HPET\n");
    return;
}