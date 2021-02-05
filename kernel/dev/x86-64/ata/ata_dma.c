/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/x86-64/ata/ata_disk.h>
#include <dev/x86-64/ata/ata_dma.h>
#include <sys/deviceapi/deviceapi_block.h>
#include <sys/devicemgr/device.h>
#include <sys/kprintf/kprintf.h>
#include <sys/string/mem.h>
#include <sys/sync/sync.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

// PRDT, maximum of 8192 entries
ata_dma_prd* prdt;

// PRDs, maximum of 15 buffers of 65536 bytes each
ata_dma_buf* bufs;

bool ata_dma_buf_avail[NUM_ATA_DMA_BUFS];

void ata_dma_init() {
    // Clear DMA buffer area
    memset((uint8_t*)CONV_PHYS_ADDR(ATA_DMA_BUF_AREA_BASE), 0, ATA_DMA_BUF_AREA_SIZE);

    // Set up pointers
    prdt = (ata_dma_prd*)CONV_PHYS_ADDR(0x00200000);
    bufs = (ata_dma_buf*)CONV_PHYS_ADDR(0x00210000);

    kprintf("ATA DMA buffers initialized\n");

    return;
}

void ata_dma_read(device_t* dev, uint64_t start, uint16_t count, BYTE* buf) {
    /*
     * Reads *count* sectors from device *dev* into *buf*, starting at LBA
     * address *start*
     */

    uint64_t bytes_to_read;
    uint64_t num_reads;

    /* 
     * To know how many distinct DMA read operations we have to make, we first
     * need to know how many bytes we're reading, so we multiply count by the
     * device sector size.  We then divide it by the size of the DMA buffer, and
     * since integer division truncates we check to see if there's a remainder;
     * if there is we add one.
     */
    bytes_to_read = count * ata_sector_size(dev);
    num_reads = (bytes_to_read / ATA_DMA_BUF_SIZE) + ((bytes_to_read % ATA_DMA_BUF_SIZE) ? 1 : 0);

    spinlock_acquire(&dma_buf_lock);
    spinlock_release(&dma_buf_lock);
}