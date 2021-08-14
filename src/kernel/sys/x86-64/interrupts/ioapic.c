/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/x86-64/acpi/madt.h>
#include <sys/x86-64/interrupts/apic.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

ioapic_t* ioapic;
uint64_t num_ioapic;

ioapic_redtbl_intpol_t ioapic_gsi_polarity(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso);
uint8_t ioapic_gsi_redirection_vector(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso);
ioapic_redtbl_trigmode_t ioapic_gsi_trigger(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso);
void ioapic_setup_redir_tables(acpi_madt_record_interrupt_source_override_t** madt_iso);

void ioapic_init(acpi_madt_record_ioapic_t** madt_ioapic) {
    acpi_madt_record_interrupt_source_override_t** madt_iso;
    uint64_t i = 0;
    uint64_t j = 0;

    ioapic = NULL;
    num_ioapic = 0;

    while (madt_ioapic[i]) {
        kprintf("\tIOAPIC %hu, ACPI ID %hu, base address 0x%llX, GSI base %lu\n", i, madt_ioapic[i]->id,
                (uint64_t)madt_ioapic[i]->address, madt_ioapic[i]->gsi_base);
        i++;
        num_ioapic++;
    }

    ioapic = (ioapic_t*)kmalloc(sizeof(ioapic_t) * i);

    for (j = 0; j < i; j++) {
        ioapic[j].acpi_id = madt_ioapic[j]->id;
        ioapic[j].irq_low = madt_ioapic[j]->gsi_base;
        ioapic[j].IOREGSEL = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address);
        ioapic[j].IOREGWIN = CONV_PHYS_ADDR((uint32_t*)(uint64_t)madt_ioapic[j]->address + 0x10);

        /* 
         * The value stored in the register is # of input pins - 1.  This is the
         * correct value, without adjustment, to add to irq_low to specify the
         * full range.  For example, if irq_low is 0, and the register value is
         * 23 (meaning 24 input pins), then we add 23 to irq_low to get an
         * irq_high of 23, and 0-23 is 24 distinct values.
         */

        ioapic[j].max_redir = IOAPICVER_EXTRACT_MAXREDIR(ioapic_register_read(ioapic[j], IOAPICVER));
        ioapic[j].irq_high = ioapic[j].max_redir + ioapic[j].irq_low;

        kprintf("\tnum, high, low: %llu, %lu, %lu\n", j, ioapic[j].irq_high, ioapic[j].irq_low);
    }

    madt_iso = acpi_enumerate_interrupt_source_override();

    i = 0;
    while (madt_iso[i]) {
        kprintf("\tIRQ %hu is remapped to GSI %lu with flags %u\n", madt_iso[i]->source, madt_iso[i]->gsi,
                madt_iso[i]->flags);
        i++;
    }

    ioapic_setup_redir_tables(madt_iso);

    return;
}

ioapic_redtbl_intpol_t ioapic_gsi_polarity(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso) {
    uint8_t i = 0;

    while (madt_iso[i]) {
        if (madt_iso[i]->gsi == gsi) {
            switch (madt_iso[i]->flags & 0b11) {
                case 0b01:
                    return IOREDTBL_INTPOL_HIGH;
                    break;
                case 0b11:
                    return IOREDTBL_INTPOL_LOW;
                    break;
                default:
                    return IOREDTBL_INTPOL_HIGH;  // TODO: This may be the wrong behavior or may need further analysis
                    break;
            }
        }

        i++;
    }
    return IOREDTBL_INTPOL_HIGH;
}

uint8_t ioapic_gsi_redirection_vector(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso) {
    uint8_t i = 0;

    while (madt_iso[i]) {
        if (madt_iso[i]->gsi == gsi) {
            return madt_iso[i]->source;
        }

        // If a source IRQ x is overridden to GSI y, then when we come to source IRQ y
        // we use the vector x, to ensure that we don't overlap vectors.

        if (madt_iso[i]->source == gsi) {
            return madt_iso[i]->gsi;
        }

        i++;
    }

    // If there's no override entry, then the vector is equal to the gsi
    return gsi;
}

ioapic_redtbl_trigmode_t ioapic_gsi_trigger(uint8_t gsi, acpi_madt_record_interrupt_source_override_t** madt_iso) {
    uint8_t i = 0;

    while (madt_iso[i]) {
        if (madt_iso[i]->gsi == gsi) {
            switch ((madt_iso[i]->flags & 0b1100) >> 2) {
                case 0b01:
                    return IOREDTBL_TRIGMODE_EDGE;
                    break;
                case 0b11:
                    return IOREDTBL_TRIGMODE_LEVEL;
                    break;
                default:
                    return IOREDTBL_TRIGMODE_EDGE;  // TODO: This may be the wrong behavior or may need further analysis
                    break;
            }
        }

        i++;
    }
    return IOREDTBL_TRIGMODE_EDGE;
}

uint32_t ioapic_register_read(ioapic_t ioapic, ioapic_registers_t reg) {
    IOAPIC_REGISTER_SELECT(ioapic, reg);

    return *(ioapic.IOREGWIN);
}

void ioapic_register_write(ioapic_t ioapic, ioapic_registers_t reg, uint32_t val) {
    IOAPIC_REGISTER_SELECT(ioapic, reg);

    *(ioapic.IOREGWIN) = val;

    return;
}

ioapic_t* ioapic_responsible_for_gsi(uint32_t gsi) {
    // Returns pointer to the IOAPIC responsible for the specified GSI

    uint64_t i = 0;

    for (i = 0; i < num_ioapic; i++) {
        kprintf("\tgsi, irq_low, irq_high: %lu, %lu, %lu\n", gsi, ioapic[i].irq_low, ioapic[i].irq_high);
        if ((ioapic[i].irq_low <= gsi) && (ioapic[i].irq_high >= gsi)) {
            kprintf("\tioapic_t address, ioapic #, address, gsi: 0x%llX, %llu, 0x%llX, %lu\n", (uint64_t)ioapic, i,
                    (uint64_t)(&(ioapic[i])), gsi);
            return &(ioapic[i]);
        }
    }

    // no responsible IOAPIC, return NULL
    return NULL;
}

void ioapic_setup_redir_tables(acpi_madt_record_interrupt_source_override_t** madt_iso) {
    uint8_t i, j;
    uint8_t cur_gsi = 0;
    uint8_t vec_redir;
    uint32_t redent_lo = 0, redent_hi = 0;

    // Loop through each IOAPIC, and then each pin
    for (i = 0; i < num_ioapic; i++) {
        for (j = 0; j <= ioapic[i].max_redir; j++) {
            redent_lo = 0;
            redent_hi = 0;

            // In the initial setup, we only worry about the legacy ISA IRQs
            if (cur_gsi <= LEGACY_IRQ_MAX) {
                vec_redir = ioapic_gsi_redirection_vector(cur_gsi, madt_iso);
            } else {
                vec_redir = cur_gsi;
            }

            kprintf("\tIRQ/GSI %hu on vector %hu\n", cur_gsi, vec_redir);

            IOAPIC_REDTBL_INTVEC_SET(redent_lo, vec_redir);
            IOAPIC_REDTBL_DELMOD_SET(redent_lo, IOREDTBL_DELMOD_FIXED);
            IOAPIC_REDTBL_DESTMOD_SET(redent_lo, IOREDTBL_DESTMOD_PHYSICAL);
            IOAPIC_REDTBL_INTPOL_SET(redent_lo, ioapic_gsi_polarity(cur_gsi, madt_iso));
            IOAPIC_REDTBL_TRIGMODE_SET(redent_lo, ioapic_gsi_trigger(cur_gsi, madt_iso));
            IOAPIC_REDTBL_INTMASK_SET(redent_lo, IOREDTBL_INTMASK_ACTIVE);

            IOAPIC_REDTBL_DEST_SET(redent_hi, 0);

            kprintf("\t\tredent_lo, redent_hi: 0x%lX, 0x%lX\n", redent_lo, redent_hi);

            ioapic_register_write(ioapic[i], IOREDTBL0_LO + (2 * j), redent_lo);
            ioapic_register_write(ioapic[i], IOREDTBL0_HI + (2 * j), redent_hi);
            cur_gsi++;
        }
    }
    return;
}