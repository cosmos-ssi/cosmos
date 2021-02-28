
The CosmOS strategy for detecting an initializing devices is below

# DeviceManager

the CosmOS device manager at `/devicemgr/devicemgr.h` contains a database of physical devices (PCI, non-PCI, etc).

# Registration

* Call the "register" function for each device type. For example `network_objectmgr_register_devices()` registers the network devices.

* The device type register function for the type calls register functions for each driver. For example

```java
void network_objectmgr_register_devices() {
    rtl8139_objectmgr_register_devices();
    ne2000_objectmgr_register_devices();
}
```

Each device driver's register function finds all instance of the specific device type, and calls `void objectmgr_register_device(struct object* dev);` to register each instance.

Device's on the PCI bus can use the PCI search functions to find device instances

```
void pci_objectmgr_search_device(pci_class,pci_subclass, vendor_id, device_id, pcideviceSearchCallback cb);
void pci_objectmgr_search_devicetype(pci_class,pci_subclass, pcideviceSearchCallback cb);
```

The callback function `typedef void (*deviceSearchCallback)(struct pci_device* dev);` is called once for each device instance found in the PCI database.

Non-PCI bus devices such as Keyboard, RTC, and RS-232 will need to use hard-coded configuration.

PCI devices generally use the `pci` field of the `device` struct to contain their appropriate `pci_device` struct.  Non-PCI devices can use this field to hold a pointer to their own custom configuration.

All devices can use the `device_data` field of the `device` struct to hold device instance specific configuration such as base ports or queues.

All devices must assign a value to the field `devicetype` from the enum `device_type`.  Additionally devices which expose an API must assign an API struct to the field `api`.   Appropriate API structs are in `kernel/devicemgr/deviceapi`


# Dynamic registration

Not all devices are fixed hardware devices.  For example:

* Disk partitions
* RAM disks
* File systems
* Swap devices

These devices should expose an attach() API, and an unattach() API which can dynamically register and unregister the devices via the device manager.  The device manager functions attach_device and detach_device are used provided for use by device attach and unattach

# Initialization

The function `void initDevices();` is used by the device manager to initialize all fixed hardware devices. The devices will have passed a pointer to a init function with this prototype:

`typedef void (*device_init)(struct object* dev);`

The DeviceMgr will assign a unique name to each device such as "serial0".

# Device Types

The enum `device_type` includes all types of devices known by CosmOS.  

```java
typedef enum object_type {
    NONE = 0x00,
    SERIAL = 0x01,
    VGA = 0x02,
    RTC = 0x03,
    KEYBOARD = 0x04,
    NIC = 0x05,  // uses NIC API
    BRIDGE = 0x06,
    USB = 0x07,
    ATA = 0x08,  // ATA controller
    PIC = 0x09,
    MOUSE = 0x0A,
    FLOPPY = 0x0B,
    SPEAKER = 0x0C,
    PIT = 0x0D,
    DSP = 0x0E,
    CMOS = 0x0F,
    DMA = 0x10,
    CPU = 0x11,
    RAMDISK = 0x12,  // uses Block API
    VNIC = 0x13,     // uses NIC API
    VBLOCK = 0x14,   // uses Block API
    DISK = 0x15,     // uses Block API
    PARALLEL = 0x16,
    BDA = 0x17,
    EBDA = 0x18,
    SWAP = 0x19,
    FILESYSTEM = 0x1A,
    PARITION_TABLE = 0x1B
} device_type;
```
Devices can be queried by type in order to find appropriate devices for higher level functions to use.  For example, a screen driver could search for VGA devices, or a storage driver could search for ATA devices. 

# Use of devices

It is not generally encouraged to include header files in the subdirectory /kernel/dev/.  Instead kernel code should acess devices by qeurying the DeviceMgr for them, by name.   For example, to query for the first RS-232 port (aka "com1"), this code can be used.

```java
	// query the device manager for the device serial0
	struct object* serial0 = objectmgr_find_device("serial0");
	// get the serial device api for the device
	struct objecttype_serial* serial_api = (struct objecttype_serial*) serial0->api;
	// get the "write" function from the api
	serial_write_function write_func = serial_api->write;
	// invoke the write function
	(*write_func)(serial0, message);	
```


