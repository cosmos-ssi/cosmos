//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _OBJECT_H
#define _OBJECT_H
#include <sys/obj/objecttype/objectype.h>
#include <types.h>

#define OBJECT_MAX_DESCRIPTION 64

struct object;

// return 1 if successful, 0 if failed to unit
typedef uint8_t (*obj_init)(struct object* obj);
// return 1 is successful, 0 if failed to uninit, including if device refcount>0
typedef uint8_t (*obj_uninit)(struct object* obj);

struct object {
    /*
     * the combination of name (from object_type_names) and index
     * create the device name.  ie "serial0".
     */
    int8_t* name;
    uint8_t type_index;
    /*
     * the type (SERIAL, VGA etc)
     */
    uint16_t objectype;
    /*
     * init function
     */
    obj_init init;
    /*
     * un-init function
     */
    obj_uninit uninit;
    /*
     * human readable description provided by the driver
     */
    uint8_t description[OBJECT_MAX_DESCRIPTION];
    /*
     * object-specific data
     */
    void* object_data;
    /*
     * For PCI devices, this is a struct pci_device*.
     */
    struct pci_device* pci;
    /*
     * pointer to the type-specific API struct
     */
    void* api;
    /*
    * reference count.  incremented when a device attaches to this device
    * and decremented when a device detaches from this device
    */
    uint8_t reference_count;
    /*
    * handle. every instance has a unique handle
    */
    uint64_t handle;
};

// new device, allocated on the kernel heap
struct object* object_new_object();
#endif