/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/logical/fs/fs_util.h>
#include <obj/x86-64/ata/ata.h>
#include <obj/x86-64/ata/ata_controller.h>
#include <obj/x86-64/ata/ata_disk.h>
#include <obj/x86-64/ata/ata_util.h>
#include <sys/debug/assert.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_block.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/sleep/sleep.h>
#include <sys/string/mem.h>

// https://wiki.osdev.org/PCI_IDE_Controller

// registers needs to be uint8_t[6]
void calculate_ida_lba_register_values(uint32_t lba, uint8_t* registers) {
    registers[0] = (lba & 0x000000FF) >> 0;
    registers[1] = (lba & 0x0000FF00) >> 8;
    registers[2] = (lba & 0x00FF0000) >> 16;
    registers[3] = (lba & 0xFF000000) >> 24;
    registers[4] = 0;
    registers[5] = 0;
}

uint32_t ata_rw(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba, bool read) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(data);
    struct ata_disk_objectdata* diskdata = (struct ata_disk_objectdata*)obj->object_data;
    struct ata_device* disk = ata_get_disk(diskdata->object, diskdata->channel, diskdata->disk);
    uint16_t sector_size = disk->bytes_per_sector;

    //	kprintf("channel %llu \n", diskdata->channel);
    //	kprintf("disk %llu \n", diskdata->disk);
    ata_select_device(diskdata->controller, diskdata->channel, diskdata->disk);

    // wait
    ata_wait_busy(diskdata->controller, diskdata->channel);

    // calc total sectors
    uint32_t total_sectors = data_size / sector_size;
    if (0 != data_size % sector_size) {
        total_sectors += 1;
    }

    // sector count
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_SECTOR_COUNT_1, 0);

    // lba
    uint8_t regs[6];
    calculate_ida_lba_register_values(start_lba, regs);
    //	kprintf("sector %llu \n", sector);
    //	kprintf("regs %llu %llu %llu %llu %llu %llu\n", regs[0],regs[1],regs[2],regs[3],regs[4],regs[5]);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_3, regs[3]);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_4, regs[4]);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_5, regs[5]);

    // sector count
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_SECTOR_COUNT_0, total_sectors);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_0, regs[0]);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_1, regs[1]);
    ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_LBA_2, regs[2]);

    // select master
    /*
     *	Bits 0 : 3: Head Number for CHS.
     *	Bit 4: Slave Bit. (0: Selecting Master Drive, 1: Selecting Slave Drive).
     *	Bit 5: Obsolete and isn't used, but should be set.
     *	Bit 6: LBA (0: CHS, 1: LBA).
     *	Bit 7: Obsolete and isn't used, but should be set.
     */
    // E0 is bits 5,6,7 set.
    if (diskdata->disk == 0) {
        // master
        ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_HDDEVSEL, 0xE0);
    } else {
        // slave
        ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_HDDEVSEL, 0xE0 | 0x10);
    }

    if (read == true) {
        // send the read command
        ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_COMMAND, ATA_CMD_READ_PIO);
    } else {
        // send the write command
        ata_register_write(diskdata->controller, diskdata->channel, ATA_REGISTER_COMMAND, ATA_CMD_WRITE_PIO);
    }

    while (ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS) & ATA_STATUS_BUSY) {
        sleep_wait(1);
    }

    //	kprintf("fff %llu\n",ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS) & ATA_STATUS_BUSY);

    uint8_t status = ata_register_read(diskdata->controller, diskdata->channel, ATA_REGISTER_STATUS);
    //	kprintf("Status: %llu\n", status);
    if (status & ATA_STATUS_ERROR) {
        kprintf("IDE Error\n");
    }
    if (status & ATA_STATUS_BUSY) {
        kprintf("IDE Busy\n");
    }

    uint16_t* buffer = (uint16_t*)data;

    uint32_t idx = 0;
    for (int j = 0; j < total_sectors; j++) {
        ata_wait_busy(diskdata->controller, diskdata->channel);
        ata_wait_drq(diskdata->controller, diskdata->channel);
        for (int i = 0; i < sector_size / 2; i++) {
            if (read == true) {
                buffer[idx++] = ata_register_read_word(diskdata->controller, diskdata->channel, ATA_REGISTER_DATA);
            } else {
                ata_register_write_word(diskdata->controller, diskdata->channel, ATA_REGISTER_DATA, buffer[idx++]);
            }
        }
    }

    //   kprintf("return %llu\n", data_size);
    return data_size;
}

uint32_t ata_read(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    //    kprintf("ata_read %llu\n", data_size);
    return ata_rw(obj, data, data_size, start_lba, true);
}

uint32_t ata_write(struct object* obj, uint8_t* data, uint32_t data_size, uint32_t start_lba) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    ASSERT_NOT_NULL(data_size);
    //    kprintf("ata_write %llu\n", data_size);

    return ata_rw(obj, data, data_size, start_lba, false);
}

uint16_t ata_sector_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct ata_disk_objectdata* diskdata = (struct ata_disk_objectdata*)obj->object_data;
    struct ata_device* disk = ata_get_disk(diskdata->object, diskdata->channel, diskdata->disk);
    ASSERT_NOT_NULL(disk);
    return disk->bytes_per_sector;
}

uint32_t ata_total_size(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    struct ata_disk_objectdata* diskdata = (struct ata_disk_objectdata*)obj->object_data;
    struct ata_device* disk = ata_get_disk(diskdata->object, diskdata->channel, diskdata->disk);
    ASSERT_NOT_NULL(disk);
    return disk->size;
}

uint8_t obj_init_ata_disk(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ata_disk_objectdata* disk = (struct ata_disk_objectdata*)obj->object_data;
    struct ata_device* dsk = ata_get_disk(disk->object, disk->channel, disk->disk);
    ASSERT_NOT_NULL(dsk);

    kprintf("Init %s serial '%s' on controller %s of size %llu (%s)\n", obj->description, dsk->serial,
            disk->object->name, dsk->size, obj->name);

    // mount partition_index tables
    fsutil_attach_partition_tables(obj);
    return 1;
}

uint8_t obj_uninit_ata_disk(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    fsutil_detach_partition_tables(obj);
    return 1;
}

void ata_register_disk(struct object* controllerDevice, uint8_t channel, uint8_t disk) {
    /*
     * register device
     */
    struct object* objectinstance = object_new_object();
    objectinstance->init = &obj_init_ata_disk;
    objectinstance->uninit = &obj_uninit_ata_disk;
    objectinstance->objectype = OBJECT_TYPE_DISK;
    objectmgr_set_object_description(objectinstance, "ATA Disk");
    /*
     * device data
     */
    struct ata_disk_objectdata* object_data = (struct ata_disk_objectdata*)kmalloc(sizeof(struct ata_disk_objectdata));
    object_data->controller = controllerDevice->object_data;
    object_data->channel = channel;
    object_data->object = controllerDevice;
    object_data->disk = disk;
    objectinstance->object_data = object_data;
    /*
     * the device api
     */
    struct objectinterface_block* api = (struct objectinterface_block*)kmalloc(sizeof(struct objectinterface_block));
    memzero((uint8_t*)api, sizeof(struct objectinterface_block));
    api->write = &ata_write;
    api->read = &ata_read;
    api->sector_size = &ata_sector_size;
    api->total_size = &ata_total_size;
    objectinstance->api = api;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}
