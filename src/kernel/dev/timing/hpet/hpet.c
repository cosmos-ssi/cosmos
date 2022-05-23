/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021-2022 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/timing/hpet/hpet.h>
#include <dev/timing/hpet/request_queue.h>
#include <subsystems.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sync/sync.h>
#include <sys/timing/timing.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/rsdt.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

SUBSYSTEM_DRIVER(hpet, "High-Precision Event Timer", "High-Precision Event Timer", "Kurt M. Weber",
                 "weberk294@gmail.com", "0.1", 0xb7a2e515a3f4c2a3, 0x5b47216f5833f1de, 0x879c22ee8cf15ff5,
                 0x7e23d05814a1cfb5, SUBSYSTEM_ID_TIMER, &hpet_init);

const uint64_t hpet_max_period = 0x05F5E100;

// The timing API takes requests in nanosecond increments; we use this to
// account for the fact that timer frequency/resolution is very likely something
// other than a nanosecond
uint64_t hpet_main_freq_divisor;

uint64_t hpet_main_counter_frequency;
hpet_main_registers_t* hpet_registers;

kernel_spinlock comparator_spinlock;

void hpet_init_useful_values();
void hpet_interrupt_irq_0();
void hpet_interrupt_irq_8();
uint64_t hpet_read_main_counter_val();
bool hpet_set_deadline_relative(timing_request_t* deadline);
void hpet_add_oneshot(uint64_t num_timer, uint64_t deadline);

void hpet_add_oneshot(uint64_t num_timer, uint64_t deadline) {
    MODULE_SPINLOCK_ACQUIRE(comparator_spinlock);

    hpet_registers->timer_registers[num_timer].comparator_value = deadline;
    hpet_registers->timer_registers[num_timer].configuration_capability |= (1 << 2);

    MODULE_SPINLOCK_RELEASE(comparator_spinlock);

    return;
}

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
    timing_driver_t* td;
    timing_source_t* sources;
    uint64_t i;

    // initialize driver spinlocks
    MODULE_SPINLOCK_INIT(comparator_spinlock);

    td = (timing_driver_t*)timing_driver;
    di = (driver_info_1_t*)(driver_list_entry->driver_info);

    kprintf("\t%s: %s driver (%s) version %s loaded.  Contact %s <%s>.\n", di->shortname, di->longname, di->description,
            di->version, di->authors, di->contact);

    acpi_hpet = acpi_find_table(ACPI_HPET);

    hpet_registers = (hpet_main_registers_t*)CONV_PHYS_ADDR(acpi_hpet->address.address);

    // For some reason, there are...issues...accessing the members of
    // general_capabilities_id (other than period, which is returned correctly)
    // via the struct.  So for now, we'll copy it into a variable and access
    // those via masks and shifts. TODO: #80 Fix it

    // hpet_general_capabilities_id_register_t* gcap = &(hpet_registers->general_capabilities_id);

    // Don't remove this comment or commented-out line because it indicates
    // something I need to work out, even if the variable itself is unused for
    // the moment.

    hpet_init_useful_values();

    // Set up the driver information struct
    td->api.calibrate = NULL;
    td->api.set_deadline_relative = hpet_set_deadline_relative;
    td->driver_id[0] = di->id[0];
    td->driver_id[1] = di->id[1];
    td->driver_id[2] = di->id[2];
    td->driver_id[3] = di->id[3];
    td->num_sources = 2;
    td->type = TIMING_SOURCE_HPET;
    td->list_entry = driver_list_entry;

    // Initialize the list of timing sources
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

    // We only use HPET timers 1 and 2, so we only bother to configure those
    for (i = 0; i < 2; i++) {
    }

    hpet_request_queue_init();

    HPET_MAIN_ENABLE(hpet_registers->general_configuration);

    HPET_LEGACY_ENABLE(hpet_registers->general_configuration);

    interrupt_router_register_interrupt_handler(0, hpet_interrupt_irq_0);
    interrupt_router_register_interrupt_handler(8, hpet_interrupt_irq_8);

    return (void*)sources;
}

void hpet_init_kernel_tick() {
    // Per the spec, first we should disable the counter to avoid missed
    // interrupts
    HPET_MAIN_DISABLE(hpet_registers->general_configuration);
    hpet_registers->main_counter_value = 0;

    // set periodic mode
    hpet_registers->timer_registers[1].configuration_capability |= (1 << 3);

    // enable interrupts on this timer
    hpet_registers->timer_registers[1].configuration_capability |= (1 << 2);

    // enable software write of counter--this will automatically be reset to 0
    // after we do the write
    hpet_registers->timer_registers[1].configuration_capability |= (1 << 6);

    // tick is 1000 Hz
    hpet_registers->timer_registers[1].comparator_value =
        hpet_registers->main_counter_value + (hpet_main_counter_frequency / 1000);
    hpet_registers->timer_registers[1].comparator_value = hpet_main_counter_frequency / 1000;

    // enable HPET
    HPET_MAIN_ENABLE(hpet_registers->general_configuration);
}

void hpet_init_useful_values() {
    // Precalculates some useful values that need to be referred to again and
    // again, and should not change for the duration of the operating system
    // session

    hpet_main_counter_frequency = hpet_calc_frequency(hpet_registers);

    // If the counter frequency is greater than one gigahertz, this will be zero
    hpet_main_freq_divisor = one_billion / hpet_main_counter_frequency;
}

void hpet_install_next_deadline(uint64_t timer) {
    if (hpet_request_queue_valid()) {
        hpet_add_oneshot(timer, hpet_request_queue_first_deadline());
    }

    return;
}

void hpet_interrupt_irq_0() {
    uint64_t current_time;
    timing_request_t** expired_requests;
    hpet_request_queue_t* queue;

    current_time = hpet_registers->main_counter_value;

    queue = hpet_request_queue_slice_deadline(current_time);
    if (!queue) {
        return;
    }

    // TODO: Call the appropriate function to process expired requests.  Not
    // needed at time of writing because the only timer requests come from
    // system_sleep(), which requires no processing as it is entirely
    // synchronous and blocking and internal to the kernel.  Once we get
    // multitasking and userspace going, that will change.
    expired_requests = hpet_request_queue_next_expired_request(queue);
    hpet_install_next_deadline(0);

    kfree(expired_requests);
    return;
}

void hpet_interrupt_irq_8() {
    //kprintf("RTC HPET\n");
    return;
}

uint64_t hpet_read_main_counter_val() {
    return hpet_registers->main_counter_value;
}

bool hpet_set_deadline_relative(timing_request_t* deadline) {
    uint64_t cur_val, deadline_val;
    uint64_t deadline_adjustment;  // this is expressed in HPET frequency units

    // If the HPET frequency is less than one gigahertz, we need to scale the delay down accordingly
    if (hpet_main_freq_divisor) {
        deadline_adjustment = deadline->delay_nsec / hpet_main_freq_divisor;
    }

    cur_val = hpet_read_main_counter_val();

    deadline_val = cur_val + deadline_adjustment;

    hpet_request_queue_add(deadline, deadline_val);

    hpet_install_next_deadline(0);

    return false;
}