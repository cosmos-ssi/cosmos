//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/block/block_device.hpp>

uint32_t readSectors(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}
uint32_t writeSectors(struct object* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}

uint16_t sectorSize(struct object* dev) {
    return 0;
}

uint32_t totalSize(struct object* dev) {
    return 0;
}
