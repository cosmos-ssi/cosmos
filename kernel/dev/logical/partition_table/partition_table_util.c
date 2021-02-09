//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/partition_table/partition_table_util.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_part_table.h>

uint32_t partition_table_util_write_sectors(struct device* partition_table_device, uint8_t partition_index,
                                            uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->device_data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    return (*pt_api->write)(partition_table_device, partition_index, data, data_size, start_lba);
}

uint32_t partition_table_util_read_sectors(struct device* partition_table_device, uint8_t partition_index,
                                           uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->device_data);
    struct deviceapi_part_table* pt_api = (struct deviceapi_part_table*)partition_table_device->api;
    return (*pt_api->read)(partition_table_device, partition_index, data, data_size, start_lba);
}

uint16_t partition_table_util_sector_size(struct device* partition_table_device, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->device_data);
    panic("not implemented");
    return 0;
}

uint32_t partition_table_util_total_size(struct device* partition_table_device, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_device);
    ASSERT(partition_table_device->devicetype == PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_device->device_data);
    panic("not implemented");
    return 0;
}