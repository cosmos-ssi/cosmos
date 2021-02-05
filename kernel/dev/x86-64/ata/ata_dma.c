/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/ata/ata_dma.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

// PRDT, maximum of 8192 entries
ata_dma_prd* prdt;

// PRDs, maximum of 15 buffers of 65536 bytes each
ata_dma_buf* bufs;

void ata_dma_init() {
    // Clear DMA buffer area
    memset((uint8_t*)CONV_PHYS_ADDR(ATA_DMA_BUF_BASE), 0, ATA_DMA_BUF_SIZE);

    // Set up pointers
    prdt = (ata_dma_prd*)CONV_PHYS_ADDR(0x00200000);
    bufs = (ata_dma_buf*)CONV_PHYS_ADDR(0x00210000);

    kprintf("ATA DMA buffers initialized\n");

    return;
}