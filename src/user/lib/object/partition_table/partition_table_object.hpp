//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _PARTITION_TABLE_OBJECT_HPP
#define _PARTITION_TABLE_OBJECT_HPP

#include <object/object.hpp>
#include <types.h>

class PartitionObject : public Object {
  public:
    uint8_t totalPartitions();
    uint64_t getPartition_lba(uint8_t partition);

    // partition type 64 byte string
    void getPartitionType(uint8_t partition, uint8_t* parititon_type, uint16_t len);
    uint64_t getSectorCount(uint8_t partition);

    /*
    * returns total bytes read
    */
    uint32_t readSectors(uint8_t partition_index, uint8_t* data, uint32_t data_size, uint32_t start_lba);
    /*
    * returns total bytes written
    */
    uint32_t writSectors(uint8_t partition_index, uint8_t* data, uint32_t data_size, uint32_t start_lba);

    // return 1 if we are ok to detach this device
    uint8_t detachable();
};

#endif