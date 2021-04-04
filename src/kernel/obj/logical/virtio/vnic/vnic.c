//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

// https://wiki.osdev.org/Virtio

#include <obj/logical/virtio/virtio.h>
#include <obj/logical/virtio/virtqueue.h>
#include <obj/logical/virtio/vnic/vnic.h>
#include <obj/x86_64/pci/devicetree.h>
#include <obj/x86_64/pci/pci_device.h>
#include <sys/asm/io.h>
#include <sys/debug/assert.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/iobuffers/iobuffers.h>
#include <sys/kmalloc/kmalloc.h>
#include <sys/kprintf/kprintf.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_nic.h>
#include <sys/obj/objectmgr/objectmgr.h>
#include <sys/obj/objecttype/objectype.h>
#include <sys/panic/panic.h>
#include <sys/string/mem.h>
#include <sys/x86_64/idt/irq.h>
#include <types.h>

uint16_t vnet_base_port;
uint8_t mac_addr[6];

inline uint32_t vnic_read_register(uint32_t reg) {
    // if 4-byte register
    if (reg < VIRTIO_QUEUE_SIZE) {
        return asm_in_d(vnet_base_port + reg);
    } else {
        // if 2-byte register
        if (reg <= VIRTIO_QUEUE_NOTIFY)
            return asm_in_w(vnet_base_port + reg);
        else  // 1-byte register
            return asm_in_b(vnet_base_port + reg);
    }
}

inline void vnic_write_register(uint32_t reg, uint32_t data) {
    // if 4-byte register
    if (reg < VIRTIO_QUEUE_SIZE) {
        asm_out_d(vnet_base_port + reg, data);
    } else {
        // if 2-byte register
        if (reg <= VIRTIO_QUEUE_NOTIFY)
            asm_out_w(vnet_base_port + reg, (uint16_t)data);
        else  // 1-byte register
            asm_out_b(vnet_base_port + reg, (uint8_t)data);
    }
}

void vnic_init_virtqueue(struct virtq** virtqueue, uint16_t queueIndex) {
    ASSERT_NOT_NULL(virtqueue);

    uint16_t queue_size = -1;

    // get queue size from the device register
    vnic_write_register(VIRTIO_QUEUE_SELECT, queueIndex);
    queue_size = (uint16_t)vnic_read_register(VIRTIO_QUEUE_SIZE);

    if (!queue_size)
        PANIC("Can't get Virtio network queue size");

    kprintf("   vnet queue %u has %u elements", queueIndex, queue_size);

    // make the queue
    struct virtq* q = virtq_new(queue_size);
    bool q_aligned = virtio_isAligned(((uint64_t)q), 4096);
    ASSERT(q_aligned);
    *virtqueue = q;

    // divide by 4096
    // The API takes a 32 bit pointer, but we have a 64 bit pointer, so ... some conversions
    uint32_t q_shifted = (uint64_t)q >> 12;

    kprintf("  Queue Address(%u): %#hX\n", queueIndex, q);

    // Write addresses (divided by 4096) to address registers
    vnic_write_register(VIRTIO_QUEUE_ADDRESS, q_shifted);
}

uint8_t get_link_status() {
    uint32_t device_status = vnic_read_register(VIRTIO_DEVICE_STATUS);
    return ((device_status & VIRTIO_NET_S_LINK_UP) == VIRTIO_NET_S_LINK_UP);
}

void print_link_status() {
    uint8_t link_status = get_link_status();
    kprintf("   link status: %lu - ", link_status);

    if (0 != link_status) {
        kprintf("UP\n");
    } else {
        kprintf("DOWN\n");
    }
}

uint8_t vnic_initialize_device(struct object* obj) {
    struct vnic_objectdata* object_data = (struct vnic_objectdata*)obj->object_data;

    // get the I/O port
    object_data->base = pci_calcbar(obj->pci);
    vnet_base_port = object_data->base;

    kprintf("Initializing virtio-net driver... Base address: %#hX\n", vnet_base_port);

    // get starting device status
    print_link_status();

    // Reset the virtio-network device
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_RESET_DEVICE);

    // Set the acknowledge status bit
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED);

    // Set the driver status bit
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED | VIRTIO_STATUS_DRIVER_LOADED);

    // Read the feature bits
    uint32_t features = vnic_read_register(VIRTIO_DEVICE_FEATURES);
    kprintf("   device features: %lX", features);

    // Make sure the features we need are supported
    if ((features & VIRTIO_NET_REQUIRED_FEATURES) != VIRTIO_NET_REQUIRED_FEATURES) {
        // uh-oh
        kprintf("Required features are not supported by virtio network device. Aborting.\n");
        vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ERROR);
    }

    // Tell the device what features we'll be using
    vnic_write_register(VIRTIO_GUEST_FEATURES, VIRTIO_NET_REQUIRED_FEATURES);

    // Tell the device the features have been negotiated
    vnic_write_register(VIRTIO_DEVICE_STATUS,
                        VIRTIO_STATUS_DEVICE_ACKNOWLEGED | VIRTIO_STATUS_DRIVER_LOADED | VIRTIO_STATUS_FEATURES_OK);

    // Make sure the device is ok with those features
    if ((vnic_read_register(VIRTIO_DEVICE_STATUS) & VIRTIO_STATUS_FEATURES_OK) != VIRTIO_STATUS_FEATURES_OK) {
        // uh-oh
        kprintf("Failed to negotiate features with virtio net device. Aborting.");
        vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ERROR);
    }

    // store the MAC address
    // TODO: store mac in the device struct
    uint16_t macReg = REG_MAC_1;
    for (int i = 0; i < 6; ++i, ++macReg)
        mac_addr[i] = (uint8_t)vnic_read_register(macReg);

    kprintf("   MAC %#hX:%#hX:%#hX:%#hX:%#hX:%#hX\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
            mac_addr[5]);

    // Init virtqueues (see 4.1.5.1.3 of virtio-v1.0-cs04.pdf)
    vnic_init_virtqueue(&(object_data->receive_queue), VIRTQ_NET_RECEIVE_INDEX);
    vnic_init_virtqueue(&(object_data->send_queue), VIRTQ_NET_TRANSMIT_INDEX);

    // Setup the receive queue
    vnic_setup_receive_buffers(object_data->receive_queue, 16);

    // Setup an interrupt handler for this device
    interrupt_router_register_interrupt_handler(obj->pci->irq, &vnic_irq_handler);
    kprintf("   init %s at IRQ %llu Vendor %#hX Device %#hX Base %#hX (%s)\n", obj->description, obj->pci->irq,
            obj->pci->vendor_id, obj->pci->device_id, object_data->base, obj->name);

    // Tell the device it's initialized
    vnic_write_register(VIRTIO_DEVICE_STATUS, VIRTIO_STATUS_DEVICE_ACKNOWLEGED | VIRTIO_STATUS_DRIVER_LOADED |
                                                  VIRTIO_STATUS_FEATURES_OK | VIRTIO_STATUS_DRIVER_READY);

    // Remind the device that it has receive buffers.
    vnic_write_register(VIRTIO_QUEUE_NOTIFY, VIRTQ_NET_RECEIVE_INDEX);

    // kprintf("\n     Requesting IP address via DHCP...");
    //ARP_SendRequest(IPv4_PackIP(10, 0, 2, 4), mac_addr);
    // DHCP_Send_Discovery(mac_addr);

    // get ending device status
    print_link_status();
    kprintf("   virtio-net driver initialized.\n");
    return 1;
}

void vnic_setup_receive_buffers(struct virtq* receiveQueue, uint8_t count) {
    const uint16_t bufferSize = 1526;  // as per virtio specs

    // Allocate and add 16 buffers to receive queue
    for (uint16_t i = 0; i < count; ++i) {
        uint8_t* buffer = kmalloc(bufferSize);
        struct virtq_descriptor* desc = virtq_descriptor_new(buffer, bufferSize, true);

        virtq_enqueue_descriptor(receiveQueue, desc);
    }

    vnic_write_register(VIRTIO_QUEUE_NOTIFY, VIRTQ_NET_RECEIVE_INDEX);
}

// the hardware raises an IRQ each time a TX frame is acknowledged, or an RX frame is ready for us.
void vnic_irq_handler(stack_frame* frame) {
    ASSERT_NOT_NULL(frame);
    kprintf("#");

    // get virtual device
    uint8_t devicename[] = {"nic0"};

    struct object* obj = objectmgr_find_object_by_name(devicename);
    if (0 == obj) {
        kprintf("Unable to find %s\n", devicename);
        return;
    }

    // get device data
    struct vnic_objectdata* object_data = (struct vnic_objectdata*)obj->object_data;

    // check for used send queues (meaning the device confirmed receipt)
    while (object_data->send_queue->used.idx != object_data->send_queue->last_seen_used) {
        kprintf("irqh: Packet sent successfully");
        struct virtq_descriptor* desc = virtq_dequeue_descriptor(object_data->send_queue);
        virtq_descriptor_delete(desc);
    }

    // see if the receive queue has been used
    while (object_data->receive_queue->used.idx != object_data->receive_queue->last_seen_used) {
        kprintf("irqh: Packet received successfully");

        // get the descriptor
        struct virtq_descriptor* desc = virtq_dequeue_descriptor(object_data->receive_queue);

        // TODO: something with the data

        // delete the descriptor
        virtq_descriptor_delete(desc);

        // restock receive queue buffer
        vnic_setup_receive_buffers(object_data->receive_queue, 1);
    }

    // EOI sent to the PIC by the interrupt handler
}

void vnic_rx(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);
    PANIC("vnic read not implemented yet");
}

void vnic_tx(struct object* obj, uint8_t* data, uint16_t size) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(data);

    kprintf("vnic_tx sending data\n");

    // Allocate a buffer for the packet & header
    uint32_t bufferSize = size + sizeof(virtio_net_hdr);
    virtio_net_hdr* netBuffer = iobuffers_request_buffer(bufferSize);

    // Set the header (basic for now - all zeros)
    memzero((uint8_t*)netBuffer, sizeof(virtio_net_hdr));

    // Copy packet to buffer
    memcpy((uint8_t*)((uint8_t*)netBuffer + sizeof(virtio_net_hdr)), (uint8_t*)data, size);

    // get the device data
    struct vnic_objectdata* object_data = (struct vnic_objectdata*)obj->object_data;

    // load a descriptor with our buffer
    struct virtq_descriptor* desc = virtq_descriptor_new((uint8_t*)netBuffer, bufferSize, false);

    // queue it up
    virtq_enqueue_descriptor(object_data->send_queue, desc);

    // tell the device we're ready to send
    vnic_write_register(VIRTIO_QUEUE_NOTIFY, VIRTQ_NET_TRANSMIT_INDEX);
}

// As part of PCI discovery, devicemgr calls this to register us as an instance of type VNIC.
// We create a device instance, attach an API and data storage, and register it.
void objectmgr_register_pci_vnic(struct pci_device* dev) {
    ASSERT_NOT_NULL(dev);

    // create a new device
    struct object* objectinstance = object_new_object();

    // bind an initialization function to the device (called by devicemgr during startup)
    objectinstance->init = &vnic_initialize_device;

    // pci device info (including IRQ)
    objectinstance->pci = dev;

    // set properties
    objectinstance->objectype = OBJECT_TYPE_VNIC;
    objectmgr_set_object_description(objectinstance, "Virtio NIC");

    // define an api
    struct objectinterface_nic* api = (struct objectinterface_nic*)kmalloc(sizeof(struct objectinterface_nic));
    memzero((uint8_t*)api, sizeof(struct objectinterface_nic));
    api->write = &vnic_tx;
    api->read = &vnic_rx;
    objectinstance->api = api;

    // reserve for device-specific data
    struct vnic_objectdata* object_data = (struct vnic_objectdata*)kmalloc(sizeof(struct vnic_objectdata));
    objectinstance->object_data = object_data;

    // register
    objectmgr_register_object(objectinstance);
}

// find all virtio ethernet devices and register them
void objectmgr_register_vnic_devices() {
    pci_objectmgr_search_device(PCI_CLASS_NETWORK, PCI_NETWORK_SUBCLASS_ETHERNET, VIRTIO_PCI_MANUFACTURER,
                                VIRTIO_PCI_DEVICED_NETWORK, &objectmgr_register_pci_vnic);
}
