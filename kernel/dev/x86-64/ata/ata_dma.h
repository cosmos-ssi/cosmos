/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_DMA_H
#define _ATA_DMA_H

#include <types.h>

// Base physical address and size of ATA DMA buffer area
#define ATA_DMA_BUF_BASE 0x200000
#define ATA_DMA_BUF_SIZE 0x100000

typedef struct ata_dma_prd {
    uint32_t buf_addr;
    uint16_t bytes;
    uint16_t reserved;
} __attribute__((packed)) ata_dma_prd;

typedef BYTE ata_dma_buf[65536];

extern ata_dma_prd* prdt;
extern ata_dma_buf* bufs;

void ata_dma_init();

#endif