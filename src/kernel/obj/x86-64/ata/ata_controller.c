/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020-2021 Kurt M. Weber                         *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/ata/ata.h>
#include <obj/x86-64/ata/ata_controller.h>
#include <obj/x86-64/ata/ata_disk.h>
#include <obj/x86-64/ata/ata_dma.h>
#include <obj/x86-64/ata/ata_identity.h>
#include <obj/x86-64/ata/ata_util.h>
#include <obj/x86-64/pci/pci.h>
#include <sys/asm/asm.h>
#include <sys/debug/assert.h>
#include <sys/objectmgr/objectmgr.h>

#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/sleep/sleep.h>
#include <sys/string/string.h>
#include <types.h>

void ata_detect_devices(struct object* object, struct ata_controller* controller);
#define IDE_SERIAL_IRQ 14

/*
 * detect all the addresses on a ATA controller
 */
void ata_detect_addresses(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT_NOT_NULL(obj->pci);

    struct ata_controller* controller = (struct ata_controller*)obj->object_data;
    uint8_t bus = obj->pci->bus;
    uint8_t device = obj->pci->device;
    uint8_t function = obj->pci->function;

    uint32_t bar_result;
    uint32_t bar_base;

    bar_result = pci_header_read_bar0(bus, device, function);
    controller->channels[ATA_PRIMARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x1F0 : bar_result);

    bar_result = pci_header_read_bar1(bus, device, function);
    controller->channels[ATA_PRIMARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x3F6 : bar_result);

    bar_result = pci_header_read_bar2(bus, device, function);
    controller->channels[ATA_SECONDARY].base_io = (((bar_result == 0) || (bar_result == 1)) ? 0x170 : bar_result);

    bar_result = pci_header_read_bar3(bus, device, function);
    controller->channels[ATA_SECONDARY].base_io_ctrl = (((bar_result == 0) || (bar_result == 1)) ? 0x376 : bar_result);

    bar_result = pci_header_read_bar4(bus, device, function);
    switch (pci_get_bar_type(bar_result)) {
        case PCI_BAR_MMIO:
            controller->channels[ATA_PRIMARY].dma_address.addr_type = ATA_DMA_ADDR_MMIO;
            controller->channels[ATA_SECONDARY].dma_address.addr_type = ATA_DMA_ADDR_MMIO;
            break;
        case PCI_BAR_PORT:
            controller->channels[ATA_PRIMARY].dma_address.addr_type = ATA_DMA_ADDR_PIO;
            controller->channels[ATA_SECONDARY].dma_address.addr_type = ATA_DMA_ADDR_PIO;
            break;
        default:
            PANIC("Invalid BAR type!");
            break;
    }

    bar_base = pci_get_bar_base(bar_result);
    controller->channels[ATA_PRIMARY].dma_address.command = bar_base;
    controller->channels[ATA_PRIMARY].dma_address.status = bar_base + 2;
    controller->channels[ATA_PRIMARY].dma_address.prdt = bar_base + 4;
    controller->channels[ATA_SECONDARY].dma_address.command = bar_base + 8;
    controller->channels[ATA_SECONDARY].dma_address.status = bar_base + 10;
    controller->channels[ATA_SECONDARY].dma_address.prdt = bar_base + 12;

    return;
}

/*
 * init ATA controller
 */
uint8_t obj_init_ata(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    struct ata_controller* controller = (struct ata_controller*)obj->object_data;

    kprintf("Init %s at IRQ %llu Vendor %#hX Device %#hX (%s)\n", obj->description, obj->pci->irq, obj->pci->vendor_id,
            obj->pci->device_id, obj->name);

    // set up selected devices
    controller->channels[ATA_PRIMARY].selected_device = ATA_DRIVE_SELECT_NONE;
    controller->channels[ATA_SECONDARY].selected_device = ATA_DRIVE_SELECT_NONE;

    // detect addresses
    ata_detect_addresses(obj);

    kprintf("    Primary IDE I/O at %#llX, control at %#llX\n", controller->channels[ATA_PRIMARY].base_io,
            controller->channels[ATA_PRIMARY].base_io_ctrl);
    kprintf("    Secondary IDE I/O at %#llX, control at %#llX\n", controller->channels[ATA_SECONDARY].base_io,
            controller->channels[ATA_SECONDARY].base_io_ctrl);

    // if this doesn't set the IRQ then this is a parallel IDE, but we don't need to know that
    pci_header_set_irq(obj->pci->bus, obj->pci->device, obj->pci->function, IDE_SERIAL_IRQ);

    // turn off interrupts
    ata_interrupt_enable(controller, ATA_PRIMARY, false);
    ata_interrupt_enable(controller, ATA_SECONDARY, false);

    ata_detect_devices(obj, controller);

    ata_dma_init();

    return 1;
}

void ata_search_cb(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);
    /*
     * register device
     */
    struct object* objectinstance = objectmgr_new_object();
    objectinstance->init = &obj_init_ata;
    objectinstance->pci = dev;
    objectinstance->devicetype = ATA;
    objectmgr_set_object_description(objectinstance, "ATA");
    /*
     * device data
     */
    struct ata_controller* object_data = (struct ata_controller*)kmalloc(sizeof(struct ata_controller));
    objectinstance->object_data = object_data;
    /*
     * register
     */
    objectmgr_register_object(objectinstance);
}

void ata_objectmgr_register_objects() {
    pci_objectmgr_search_devicetype(PCI_CLASS_MASS_STORAGE, PCI_MASS_STORAGE_SUBCLASS_IDE, &ata_search_cb);
}

void ata_detect_devices(struct object* object, struct ata_controller* controller) {
    uint8_t i, j;
    uint8_t status;
    //  struct ata_device_t* tmp;

    for (i = 0; i < 2; i++) {
        controller->channels[i].selected_device = ATA_DRIVE_SELECT_NONE;
        for (j = 0; j < 2; j++) {
            ata_select_device(controller, i, j);

            ata_register_write(controller, i, ATA_REGISTER_COMMAND, ATA_COMMAND_IDENTIFY);
            sleep_wait(1);

            status = ata_register_read(controller, i, ATA_REGISTER_STATUS);
            if (!status) {  // no device found
                controller->channels[i].devices[j].exists = false;
                continue;
            }

            // set the exists flag to true
            controller->channels[i].devices[j].exists = true;

            // status isn't 0, so the device exists--so we poll until ready
            while (1) {
                status = ata_register_read(controller, i, ATA_REGISTER_STATUS);

                if (status & ATA_STATUS_ERROR) {
                    // do ATAPI identify, it requires a packet request
                    ata_detect_atapi(controller, i);
                    break;
                } else if (!(status & ATA_STATUS_BUSY) && (status & ATA_STATUS_DRQ)) {
                    break;
                }
            }

            controller->channels[i].devices[j].identity = ata_detect_read_identify(controller, i);
            ata_extract_identity(controller->channels[i].devices[j].identity, &(controller->channels[i].devices[j]));

            // register the device
            //	kprintf("    Found disk at channel %llu, device %llu\n",i,j);
            ata_register_disk(object, i, j);
        }
    }
    return;
}

struct ata_device* ata_get_disk(struct object* obj, uint8_t channel, uint8_t disk) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->object_data);
    ASSERT(((channel >= 0) && (channel <= 1)));
    ASSERT(((disk >= 0) && (disk <= 1)));
    struct ata_controller* controller = (struct ata_controller*)obj->object_data;
    return &(controller->channels[channel].devices[disk]);
}
