/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

/*
 * this is a disk device, such as an HD or a CDROM
 */
#ifndef _ATA_DISK_H
#define _ATA_DISK_H

#include <types.h>

struct object;

typedef struct ata_disk_objectdata {
    struct object* object;
    struct ata_controller* controller;
    uint8_t channel;
    uint8_t disk;
} __attribute__((packed)) ata_disk_objectdata;

/*
 * a disk only needs one function; a registration called by the controller
 */
void ata_register_disk(struct object* controllerDevice, uint8_t channel, uint8_t disk);
uint16_t ata_sector_size(struct object* obj);

#endif