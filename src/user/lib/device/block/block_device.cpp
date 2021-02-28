//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/block/block_device.hpp>

uint32_t readSectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}
uint32_t writeSectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}

uint16_t sectorSize(struct device* dev) {
    return 0;
}

uint32_t totalSize(struct device* dev) {
    return 0;
}
