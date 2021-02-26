//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _BLOCK_DEVICE_HPP
#define _BLOCK_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

class BlockDevice : public Device {
  public:
    uint32_t block_read_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
    uint32_t block_write_sectors(struct device* dev, uint8_t* data, uint32_t data_size, uint32_t start_lba);
    uint16_t block_sector_size(struct device* dev);
    uint32_t block_total_size(struct device* dev);
};

#endif