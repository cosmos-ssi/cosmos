/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/x86-64/acpi/acpi.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic.h>
#include <types.h>

apic_register_t* acpi_get_local_apic_address() {
    // this returns the PHYSICAL address
    acpi_madt_t* madt;

    madt = (acpi_madt_t*)acpi_find_table(ACPI_MADT);

    return (void*)(uint64_t)(madt->local_apic_address);
}

acpi_madt_record_ioapic_t** acpi_enumerate_ioapic() {
    acpi_madt_record_ioapic_t** ret = 0;
    acpi_madt_t* madt = 0;
    acpi_madt_record_prologue_t* tmp_pro;
    // Because of pointer arithmetic rules, we have to work with pointers to
    // BYTE in order to correctly advance loop counters...yeah, it's a mess.
    BYTE* record_loc;
    uint8_t num_found = 0;

    madt = (acpi_madt_t*)acpi_find_table(ACPI_MADT);
    ASSERT_NOT_NULL(madt);

    ret = (acpi_madt_record_ioapic_t**)kmalloc(sizeof(acpi_madt_record_ioapic_t*));
    ret[0] = 0;

    record_loc = (BYTE*)madt + sizeof(*madt);

    for (record_loc = ((BYTE*)madt + sizeof(*madt)); record_loc < ((uint8_t*)madt + madt->header.length);
         record_loc += tmp_pro->length) {
        tmp_pro = (acpi_madt_record_prologue_t*)record_loc;

        if (tmp_pro->type == ACPI_MADT_RECORD_IOAPIC) {
            num_found++;

            ret = (acpi_madt_record_ioapic_t**)krealloc(ret, sizeof(acpi_madt_record_ioapic_t*) * num_found);
            ret[num_found - 1] = (acpi_madt_record_ioapic_t*)((BYTE*)tmp_pro + sizeof(acpi_madt_record_prologue_t));
            ret[num_found] = 0;
        }
    }

    return ret;
}