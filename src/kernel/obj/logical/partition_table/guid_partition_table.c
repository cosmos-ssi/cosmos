//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/GPT

#include <obj/logical/fs/block_util.h>
#include <obj/logical/fs/fs_util.h>
#include <obj/logical/partition_table/guid_partition_table.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_part_table.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/string/mem.h>
#include <sys/string/string.h>

#define GUID_PT_HEADER_LBA 1

const uint8_t GUID_PT_EFI_PART[] = {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};

uint8_t guid_pt_part_table_total_partitions(struct object* obj);
uint64_t guid_pt_part_table_get_partition_lba(struct object* obj, uint8_t partition);
uint64_t guid_part_table_get_sector_count_function(struct object* obj, uint8_t partition);

struct guid_pt_objectdata {
    struct object* block_device;
} __attribute__((packed));

void guid_pt_read_guid_pt_header(struct object* obj, struct guid_pt_header* header) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(header);
    blockutil_read(obj, (uint8_t*)header, sizeof(struct guid_pt_header), GUID_PT_HEADER_LBA, 0);
}

/*
 * perform device instance specific init here
 */
uint8_t guid_pt_init(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    struct guid_pt_header header;
    guid_pt_read_guid_pt_header(object_data->block_device, &header);
    if (header.magic[0] != GUID_PT_EFI_PART[0]) {
        return 0;
    }
    if (header.magic[1] != GUID_PT_EFI_PART[1]) {
        return 0;
    }
    if (header.magic[2] != GUID_PT_EFI_PART[2]) {
        return 0;
    }
    if (header.magic[3] != GUID_PT_EFI_PART[3]) {
        return 0;
    }
    if (header.magic[4] != GUID_PT_EFI_PART[4]) {
        return 0;
    }
    if (header.magic[5] != GUID_PT_EFI_PART[5]) {
        return 0;
    }
    if (header.magic[6] != GUID_PT_EFI_PART[6]) {
        return 0;
    }
    if (header.magic[7] != GUID_PT_EFI_PART[7]) {
        return 0;
    }
    kprintf("Init %s on %s (%s)\n", obj->description, object_data->block_device->name, obj->name);

    // attach partitions
    fsutil_attach_partitions(obj);
    return 1;
}

/*
 * perform device instance specific uninit here, like removing API structs and Device data
 */
uint8_t guid_pt_uninit(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    // struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    // kprintf("Uninit %s on %s (%s)\n", obj->description, object_data->block_device->name, obj->name);
    /*
     * unmount partitions
     */
    fsutil_detach_partitions(obj);
    /*
     * done w device
     */
    kfree(obj->api);
    kfree(obj->object_data);
    return 1;
}

/*
* read a specific partition_index table entry
*/
void guid_pt_read_guid_pt_entry(struct object* obj, struct guid_pt_entry* entry, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(entry);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    /*
    * read the GPT header
    */
    struct guid_pt_header header;
    memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    guid_pt_read_guid_pt_header(object_data->block_device, &header);

    /*
    * offset of 1st byte of partition_index entry, starting from the start of the partition_index data array
    */
    uint64_t sector_size = blockutil_get_sector_size(object_data->block_device);
    uint64_t offset = partition * sizeof(struct guid_pt_entry);
    uint64_t sector = offset / sector_size;
    uint64_t remainder = offset - (sector * sector_size);
    //   kprintf("offset %llu, sector %llu, remainder %llu\n", offset, sector, remainder);
    /*
    * read the sector
    */
    uint8_t data[sector_size];
    blockutil_read(object_data->block_device, data, sector_size, header.gpt_array_lba + sector, 0);
    memcpy((uint8_t*)entry, &(data[remainder]), sizeof(struct guid_pt_entry));

    //   debug_show_memblock(entry, sizeof(struct guid_pt_entry));
}

uint64_t guid_part_table_get_sector_count_function(struct object* obj, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(obj, &entry, partition);
    /*
    * done
    */
    return entry.end_lba - entry.start_lba;
}

uint64_t guid_pt_part_table_get_partition_lba(struct object* obj, uint8_t partition) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(obj, &entry, partition);

    /*
    * done
    */
    return entry.start_lba;
}

void guid_pt_part_table_get_partition_type(struct object* obj, uint8_t partition, uint8_t* parititon_type,
                                           uint16_t len) {
    ASSERT(partition >= 0);
    ASSERT_NOT_NULL(parititon_type);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //  struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    /*
    * read the partition entry
    */
    struct guid_pt_entry entry;
    memzero((uint8_t*)&entry, sizeof(struct guid_pt_entry));
    guid_pt_read_guid_pt_entry(obj, &entry, partition);
    /*
    * string
    */
    guid_partition_type_to_string((uint8_t*)&(entry.partition_type), parititon_type, len);
}

uint8_t guid_pt_part_table_total_partitions(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;

    /*
    * read the GPT header
    */
    struct guid_pt_header header;
    memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    guid_pt_read_guid_pt_header(object_data->block_device, &header);

    /*
    * return result
    */
    return header.num_partitions;
}

uint8_t guid_part_table_detachable(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    //   struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    return 1;
}

uint32_t guid_part_read_sectors(struct object* obj, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                                uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    uint64_t lba = guid_pt_part_table_get_partition_lba(obj, partition_index);
    return blockutil_read(object_data->block_device, data, data_size, lba + start_lba, 0);
}

uint32_t guid_part_write_sectors(struct object* obj, uint8_t partition_index, uint8_t* data, uint32_t data_size,
                                 uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    uint64_t lba = guid_pt_part_table_get_partition_lba(obj, partition_index);
    return blockutil_write(object_data->block_device, data, data_size, lba + start_lba, 0);
}

uint16_t guid_part_table_sector_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    return blockutil_get_sector_size(object_data->block_device);
}

struct object* guid_pt_attach(struct object* block_device) {
    ASSERT_NOT_NULL(block_device);
    ASSERT(1 == blockutil_is_block_object(block_device));
    ASSERT(sizeof(struct guid_pt_entry) == 128);

    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &guid_pt_init;
    objectinstance->uninit = &guid_pt_uninit;
    objectinstance->pci = 0;
    objectinstance->objectype = OBJECT_TYPE_PARTITION_TABLE;
    objectmgr_set_object_description(objectinstance, "GUID partition table");
    /*
     * the device api
     */
    struct objectinterface_part_table* api =
        (struct objectinterface_part_table*)kmalloc(sizeof(struct objectinterface_part_table));
    memzero((uint8_t*)api, sizeof(struct objectinterface_part_table));
    api->partitions = &guid_pt_part_table_total_partitions;
    api->lba = &guid_pt_part_table_get_partition_lba;
    api->type = &guid_pt_part_table_get_partition_type;
    api->sectors = &guid_part_table_get_sector_count_function;
    api->detachable = &guid_part_table_detachable;
    api->read = &guid_part_read_sectors;
    api->write = &guid_part_write_sectors;
    api->sector_size = &guid_part_table_sector_size;
    objectinstance->api = api;
    /*
     * device data
     */
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)kmalloc(sizeof(struct guid_pt_objectdata));
    object_data->block_device = block_device;
    objectinstance->object_data = object_data;
    /*
     * register
     */
    if (0 != objectmgr_attach_object(objectinstance)) {
        /*
        * increase ref count of underlying device
        */
        objectmgr_increment_object_refcount(block_device);
        /*
        * return device
        */
        return objectinstance;
    } else {
        kfree(object_data);
        kfree(api);
        kfree(objectinstance);
        return 0;
    }
}

void guid_pt_detach(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    /*
    * decrease ref count of underlying device
    */
    objectmgr_decrement_object_refcount(object_data->block_device);
    /*
    * detach
    */
    objectmgr_detach_object(obj);
}

void guid_pt_dump(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct guid_pt_objectdata* object_data = (struct guid_pt_objectdata*)obj->object_data;
    /*
    * read the GPT header
    */
    struct guid_pt_header header;
    memzero((uint8_t*)&header, sizeof(struct guid_pt_header));
    guid_pt_read_guid_pt_header(object_data->block_device, &header);
    /*
    * start the dump
    */
    kprintf("   ");
    for (uint8_t i = 0; i < 8; i++) {
        kprintf("%#llX ", header.magic[i]);
    }
    kprintf("\n");
    kprintf("   gpt_revision %llu\n", header.gpt_revision);
    kprintf("   header_size %llu\n", header.header_size);
    kprintf("   gpt_lba %llu\n", header.gpt_lba);
    kprintf("   alt_gpt_lba %llu\n", header.alt_gpt_lba);
    kprintf("   first_block %llu\n", header.first_block);
    kprintf("   last_block %llu\n", header.last_block);
    kprintf("   gpt_array_lba %llu\n", header.gpt_array_lba);
    kprintf("   num_partitions %llu\n", header.num_partitions);
    kprintf("   entry_size %llu\n", header.entry_size);
}

void guid_partition_type_to_string(uint8_t* type, uint8_t* string, uint16_t len) {
    ASSERT_NOT_NULL(type);
    ASSERT_NOT_NULL(string);
    memzero(string, len);
    ASSERT(len >= 37);  // total length we need
    uint8_t i = 0;

    uitoa3(type[3], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[2], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[1], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[0], &(string[i]), 2, 16);
    i += 2;
    string[i] = '-';
    i += 1;
    uitoa3(type[5], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[4], &(string[i]), 2, 16);
    i += 2;
    string[i] = '-';
    i += 1;
    uitoa3(type[7], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[6], &(string[i]), 2, 16);
    i += 2;
    string[i] = '-';
    i += 1;
    uitoa3(type[8], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[9], &(string[i]), 2, 16);
    i += 2;
    string[i] = '-';
    i += 1;
    uitoa3(type[10], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[11], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[12], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[13], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[14], &(string[i]), 2, 16);
    i += 2;
    uitoa3(type[15], &(string[i]), 2, 16);
    i += 2;
    ASSERT(i == 36);  // total length we need, minus null
}
