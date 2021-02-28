/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2021 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/ata/ata_disk.h>
#include <obj/x86-64/ata/ata_dma.h>
#include <sys/collection/linkedlist/linkedlist.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/string/mem.h>
#include <sys/sync/sync.h>
#include <sys/x86-64/mm/pagetables.h>
#include <types.h>

struct object;

// PRDT, two channels with 16 possible entries each
prdt* ata_dma_prdt;

// Buffers, two channels, each with 16 buffers of 65536 bytes
ata_dma_buf* bufs;

bool ata_dma_buf_avail[NUM_ATA_DMA_BUFS];

linkedlist* dma_jobs;
linkedlist* dma_ops;

void ata_dma_add_job(struct object* obj, uint64_t start, uint64_t count, BYTE* buf, ata_dma_direction dir) {
    ata_dma_job* cur_job;

    // Add the job queue entry
    cur_job = kmalloc(sizeof(ata_dma_job));
    cur_job->buf = buf;
    cur_job->obj = obj;
    cur_job->dir = dir;
    cur_job->sectors_total = count;
    cur_job->start_sector = start;

    spinlock_acquire(&dma_list_lock);
    if (!dma_jobs) {
        dma_jobs = linkedlist_new();
        dma_jobs->data = (void*)cur_job;
    } else {
        linkedlist_add(dma_jobs, (void*)cur_job);
    }
    spinlock_release(&dma_list_lock);

    return;
}

void ata_dma_execute_job() {
    //cur_job = dma_jobs;

    spinlock_acquire(&dma_buf_lock);
    spinlock_release(&dma_buf_lock);
}

void ata_dma_init() {
    // Clear DMA buffer area
    memset((uint8_t*)CONV_PHYS_ADDR(ATA_DMA_BUF_AREA_BASE), 0, ATA_DMA_BUF_AREA_SIZE);

    // Set up pointers
    ata_dma_prdt = (prdt*)iobuffers_request_buffer(sizeof(prdt));
    bufs = (ata_dma_buf*)CONV_PHYS_ADDR(0x00200000);

    dma_jobs = 0;
    dma_ops = 0;

    kprintf("ATA DMA buffers initialized\n");

    return;
}

void ata_dma_read(struct object* obj, uint64_t start, uint64_t count, BYTE* buf) {
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
    bytes_to_read = count * ata_sector_size(obj);
    num_reads = (bytes_to_read / ATA_DMA_BUF_SIZE) + ((bytes_to_read % ATA_DMA_BUF_SIZE) ? 1 : 0);

    ata_dma_add_job(obj, start, count, buf, ATA_DMA_DIR_READ);

    return;
}