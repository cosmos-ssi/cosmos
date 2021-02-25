//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/block/block_device.hpp>

uint32_t block_read_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}
uint32_t block_write_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}

uint16_t block_sector_size(struct device* dev) {
    return 0;
}

uint32_t block_total_size(struct device* dev) {
    return 0;
}
