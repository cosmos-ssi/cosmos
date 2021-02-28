//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <device/partition_table/partition_table_device.hpp>

uint8_t PartitionTable ::totalPartitions() {
    return 0;
}
uint64_t PartitionTable ::getPartition_lba(uint8_t partition) {
    return 0;
}

// partition type 64 byte string
void PartitionTable ::getPartitionType(uint8_t partition, uint8_t* parititon_type, uint16_t len) {}

uint64_t PartitionTable ::getSectorCount(uint8_t partition) {
    return 0;
}

/*
    * returns total bytes read
    */
uint32_t PartitionTable ::readSectors(uint8_t partition_index, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}
/*
    * returns total bytes written
    */
uint32_t PartitionTable ::writSectors(uint8_t partition_index, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    return 0;
}

// return 1 if we are ok to detach this device
uint8_t PartitionTable ::detachable() {
    return 0;
}