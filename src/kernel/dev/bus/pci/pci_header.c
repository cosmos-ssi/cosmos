/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <dev/bus/pci/devicetree.h>
#include <dev/bus/pci/pci.h>
#include <dev/bus/pci/pci_device.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/panic/panic.h>
#include <types.h>

uint32_t pci_get_bar_base(uint32_t bar) {
    /*
     * This function does not handle 64-bit address space BARs, since they
     * require two separate BARs.  Instead, it returns 0 if a 64-bit BAR is
     * encountered.
     */
    switch (pci_get_bar_type(bar)) {
        case PCI_BAR_PORT:
            return (bar & 0xFFFFFFFC);
        case PCI_BAR_MMIO:
            switch ((bar & 0x6) >> 1) {
                case 0:  // 32-bit
                    return (bar & 0xFFFFFFF0);
                case 1:  // 16-bit
                    return (bar & 0xFFF0);
                default:
                    return 0;
            }
        default:
            PANIC("Invalid BAR type!");
            __builtin_unreachable();
    }
}

pci_bar_type pci_get_bar_type(uint32_t bar) {
    /*
     * Determines whether the BAR is referencing a memory-mapped or port-mapped
     * IO address
     */
    if (bar & 0x1) {
        return PCI_BAR_PORT;
    } else {
        return PCI_BAR_MMIO;
    }
}

uint32_t pci_header_read_bar0(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR0_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

uint32_t pci_header_read_bar1(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR1_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

uint32_t pci_header_read_bar2(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR2_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

uint32_t pci_header_read_bar3(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR3_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

uint32_t pci_header_read_bar4(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR4_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

uint32_t pci_header_read_bar5(uint8_t bus, uint8_t device, uint8_t function) {
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, PCI_BAR5_OFFSET, 1));
    return asm_in_d(PCI_CONFIG_DATA_PORT);
}

pci_class_codes pci_header_read_class(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    // class is found in dword at offset 0x08 in all header types
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x08, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (pci_class_codes)(register_dword >> 24);
}

uint16_t pci_header_read_device_id(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (uint16_t)(register_dword >> 16);
}

uint8_t pci_header_read_irq(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    // interrupt line is found in dword at offset 0x3C in all header types
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x3C, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (pci_class_codes)(register_dword & 0x000000FF);
}

uint8_t pci_header_read_subclass(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    // class is found in dword at offset 0x08 in all header types
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x08, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (pci_class_codes)((register_dword >> 16) & 0x00FF);
}

uint8_t pci_header_read_type(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    // header type is found in dword at offset 0x0C
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x0C, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (uint8_t)((register_dword >> 16) & 0x00FF);
}

uint16_t pci_header_read_vendor(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t register_dword;
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0, 1));
    register_dword = asm_in_d(PCI_CONFIG_DATA_PORT);
    return (uint16_t)(register_dword & 0x0000FFFF);
}

void pci_header_set_irq(uint8_t bus, uint8_t device, uint8_t function, uint8_t irq) {
    // interrupt line is found in dword at offset 0x3C in all header types
    asm_out_d(PCI_CONFIG_ADDRESS_PORT, pci_config_address_build(bus, device, function, 0x3C, 1));
    asm_out_b(PCI_CONFIG_DATA_PORT, irq);
    return;
}

// https://wiki.osdev.org/PCI
uint64_t pci_calcbar(struct pci_device* pci_dev) {
    ASSERT_NOT_NULL(pci_dev);
    uint64_t bar0 = pci_header_read_bar0(pci_dev->bus, pci_dev->device, pci_dev->function);
    uint64_t bar1 = pci_header_read_bar0(pci_dev->bus, pci_dev->device, pci_dev->function);

    return ((bar0 & 0xFFFFFFF0) + ((bar1 & 0xFFFFFFFF) << 32));
}
