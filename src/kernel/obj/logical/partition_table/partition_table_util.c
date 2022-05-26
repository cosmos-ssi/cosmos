//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#if 0

#include <obj/logical/partition_table/partition_table_util.h>
#include <sys/debug/assert.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_part_table.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/panic/panic.h>

uint32_t partition_table_util_write_sectors(struct object* partition_table_object, uint8_t partition_index,
                                            uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(partition_table_object);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    ASSERT(partition_table_object->objectype == OBJECT_TYPE_PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_object->object_data);
    struct objectinterface_part_table* pt_api = (struct objectinterface_part_table*)partition_table_object->api;
    return (*pt_api->write)(partition_table_object, partition_index, data, data_size, start_lba);
}

uint32_t partition_table_util_read_sectors(struct object* partition_table_object, uint8_t partition_index,
                                           uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(partition_table_object);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);

    ASSERT(partition_table_object->objectype == OBJECT_TYPE_PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_object->object_data);
    struct objectinterface_part_table* pt_api = (struct objectinterface_part_table*)partition_table_object->api;
    return (*pt_api->read)(partition_table_object, partition_index, data, data_size, start_lba);
}

uint16_t partition_table_util_sector_size(struct object* partition_table_object, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_object);
    ASSERT(partition_table_object->objectype == OBJECT_TYPE_PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_object->object_data);
    struct objectinterface_part_table* pt_api = (struct objectinterface_part_table*)partition_table_object->api;
    return (*pt_api->sector_size)(partition_table_object);
}

uint64_t partition_table_util_get_sector_count(struct object* partition_table_object, uint8_t partition) {
    ASSERT_NOT_NULL(partition_table_object);
    ASSERT(partition_table_object->objectype == OBJECT_TYPE_PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_object->object_data);
    struct objectinterface_part_table* pt_api = (struct objectinterface_part_table*)partition_table_object->api;
    return (*pt_api->sectors)(partition_table_object, partition);
}

uint32_t partition_table_util_total_size(struct object* partition_table_object, uint8_t partition_index) {
    ASSERT_NOT_NULL(partition_table_object);
    ASSERT(partition_table_object->objectype == OBJECT_TYPE_PARTITION_TABLE);
    ASSERT_NOT_NULL(partition_table_object->object_data);
    uint16_t sector_size = partition_table_util_sector_size(partition_table_object, partition_index);
    uint64_t sectors = partition_table_util_get_sector_count(partition_table_object, partition_index);
    return sector_size * sectors;
}

#endif