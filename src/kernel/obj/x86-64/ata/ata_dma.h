/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _ATA_DMA_H
#define _ATA_DMA_H

#include <types.h>

struct object;

// Base physical address and size of ATA DMA buffer area
#define ATA_DMA_BUF_AREA_BASE 0x200000
#define ATA_DMA_BUF_AREA_SIZE 0x200000

// Size of a single DMA buffer
#define ATA_DMA_BUF_SIZE 0x10000

// Number of ATA DMA buffers
#define NUM_ATA_DMA_BUFS 32

typedef enum ata_dma_address_types { ATA_DMA_ADDR_PIO, ATA_DMA_ADDR_MMIO } ata_dma_address_types;

typedef enum ata_dma_direction { ATA_DMA_DIR_READ, ATA_DMA_DIR_WRITE } ata_dma_direction;

typedef struct ata_dma_address {
    ata_dma_address_types addr_type;
    uint32_t command;
    uint32_t status;
    uint32_t prdt;
} ata_dma_address;

typedef struct ata_dma_job {
    struct object* obj;
    uint64_t sectors_total;
    uint64_t start_sector;
    BYTE* buf;
    ata_dma_direction dir;
} ata_dma_job;

typedef struct ata_dma_op {
    ata_dma_direction dir;
    uint8_t prd_start_idx;
    uint8_t prd_end_idx;
} ata_dma_op;

typedef struct ata_dma_prd {
    uint32_t buf_addr;
    uint16_t bytes;
    uint16_t reserved;
} __attribute__((packed)) ata_dma_prd;

typedef BYTE ata_dma_buf[2][16][65536];
typedef ata_dma_prd prdt[2][16];

extern prdt* ata_dma_prdt;
extern ata_dma_buf* bufs;

void ata_dma_add_job(struct object* obj, uint64_t start, uint64_t count, BYTE* buf, ata_dma_direction dir);
void ata_dma_init();
void ata_dma_read(struct object* obj, uint64_t start, uint64_t count, BYTE* buf);

#endif