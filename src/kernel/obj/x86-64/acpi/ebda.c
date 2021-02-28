//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <obj/x86-64/bda/bda.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/debug/debug.h>
#include <sys/obj/objectmgr/objectmgr.h>

#include <sys/kprintf/kprintf.h>
#include <sys/obj/objectinterface/objectinterface_bda.h>

const uint8_t EBDA_RSD_PTR[] = {"RSD PTR "};
const uint8_t EBDA_RSD_PTR_SIZE = 8;

/*
 * size in kb
 */
uint64_t ebda_get_size() {
    uint64_t ebda_address = bda_get_ebda_address();
    return ((uint8_t*)ebda_address)[0];
}

uint8_t ebda_matches_EBDA_RSD_PTR(uint8_t* x) {
    for (uint8_t i = 0; i < EBDA_RSD_PTR_SIZE; i++) {
        if (x[i] != EBDA_RSD_PTR[i]) {
            return 0;
        }
    }
    return 1;
    //    kprintf("x: %#llX %#llX %#llX %#llX %#llX %#llX %#llX %#llX\n", x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);
    //    kprintf("EBDA_RSD_PTR: %#llX %#llX %#llX %#llX %#llX %#llX %#llX %#llX\n", EBDA_RSD_PTR[0], EBDA_RSD_PTR[1], EBDA_RSD_PTR[2], EBDA_RSD_PTR[3], EBDA_RSD_PTR[4], EBDA_RSD_PTR[5], EBDA_RSD_PTR[6], EBDA_RSD_PTR[7]);
}

uint64_t ebda_get_rsdp_top() {
    for (uint64_t i = 0x000E0000; i < 0x000FFFFF; i++) {
        uint8_t* x = (uint8_t*)i;
        if (ebda_matches_EBDA_RSD_PTR(x)) {
            return i;
        }
    }
    return 0;
}

uint64_t ebda_get_rsdp_ebda() {
    uint64_t ebda_address = bda_get_ebda_address();
    uint8_t* ebda = ((uint8_t*)ebda_address);
    for (uint32_t i = 0; i < ebda_get_size() * 1024; i++) {
        for (uint8_t j = 0; j < EBDA_RSD_PTR_SIZE; j++) {
            if (ebda[i + j] != EBDA_RSD_PTR[j]) {
                break;
            }
            return (uint64_t) & (ebda[i]);
        }
    }
    return 0;
}

struct rsdp_descriptor_2* ebda_get_rsdp() {
    uint64_t loc = ebda_get_rsdp_ebda();
    if (0 == loc) {
        loc = ebda_get_rsdp_top();
        // kprintf("%#llX\n", loc);
        // debug_show_memblock((uint8_t*)loc, 64);
    }
    return (struct rsdp_descriptor_2*)loc;
}
