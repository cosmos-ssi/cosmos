/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2019-2020 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/asm/asm.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/x86-64/mm/mm.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

uint64_t future_pt_expansion[3];
uint8_t* system_gdt;

void move_gdt();
void setup_tss();

void mmu_init() {
    int_15_map* map;
    uint8_t num_blocks, lrg_block;
    //  uint8_t i;
    //   mem_block* b;
    page_directory_t* page_directory_start;

    brk = &_end;
    //   kprintf("   brk: 0x%llX\n", (uint64_t)brk);

    system_cr3 = asm_cr3_read();

    /*
     * ISA DMA buffers need to be in lower 64MB of RAM and page aligned
     */
    //  io_buf = find_aligned_after(brk, IOBUFFERS_ALIGNMENT);
    //  kprintf("   Reserved IO memory of size %#hX at %#llX\n", IOBUFFERS_TOTAL_SIZE, io_buf);
    //  brk = io_buf + IOBUFFERS_TOTAL_SIZE;

    kmalloc_init();

    map = read_int_15_map(&num_blocks, &lrg_block);

    page_directory_start = (page_directory_t*)setup_direct_map(map, num_blocks);

    setup_page_directory(page_directory_start, map, num_blocks);

    reserve_next_ptt(PDP, future_pt_expansion);
    reserve_next_ptt(PD, future_pt_expansion);
    reserve_next_ptt(PT, future_pt_expansion);

    move_gdt();

    setup_tss();

    return;
}

void move_gdt() {
    // Move GDT to an address in direct map area. We don't actually move the GDT
    // itself, we just change the pointer in the GDTR register to point to the
    // direct-map equivalent of the physical/identity-mapped address set by the
    // bootloader.  This way we can replace the identity-map area when we set up
    // page tables for user processes.

    uint16_t gdt_len;
    uint64_t gdt_base;

    system_gdt = asm_sgdt();

    gdt_len = *((uint16_t*)system_gdt);
    gdt_base = *((uint64_t*)&system_gdt[2]);
    gdt_base = (uint64_t)CONV_PHYS_ADDR(gdt_base);
    memcpy(system_gdt, (uint8_t*)&gdt_len, sizeof(uint16_t));
    memcpy(&system_gdt[2], (uint8_t*)&gdt_base, sizeof(uint64_t));

    asm_lgdt(system_gdt);
}

void setup_tss() {
    void* gdt_base = 0;
    tss64_t tss;

    memcpy((uint8_t*)&gdt_base, &system_gdt[2], sizeof(void*));
    kprintf("gdt_base: 0x%llX\n", (uint64_t)gdt_base);

    gdt_base += TSS_SELECTOR;  // TSS area starts at byte offset 40 (0x28)
    kprintf("gdt_base: 0x%llX\n", (uint64_t)gdt_base);

    memset((uint8_t*)&tss, 0, sizeof(tss64_t));

    tss.rsp0 = 0;
    tss.rsp1 = 0;
    tss.rsp2 = 0;

    //memcpy((uint8_t*)gdt_base, (uint8_t*)&tss, sizeof(tss64_t));

    return;
}