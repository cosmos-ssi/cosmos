//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/fs/fs_facade.h>
#include <sys/kprintf/kprintf.h>
#include <sys/objectmgr/objectregistry.h>
#include <sys/objectmgr/objecttypes.h>
#include <sys/objecttype/objecttype_filesystem.h>
#include <sys/string/string.h>

void deviceregistry_init() {
    /*
    * init the types
    */
    devicetypes_init();
}

/*
* register a device
*/
void deviceregistry_registerdevice(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->devicetype);

    /*
    * get the list for the device type
    */
    struct arraylist* lst = devicetypes_get_devicelist(dev->devicetype);
    if (0 == lst) {
        lst = arraylist_new();
        devicetypes_set_devicelist(dev->devicetype, lst);
    }
    /*
    * add to the list
    */
    arraylist_add(lst, dev);
}

/*
* unregister a device
*/
void deviceregistry_unregisterdevice(struct object* dev) {
    ASSERT_NOT_NULL(dev);
    ASSERT_NOT_NULL(dev->devicetype);
    /*
    * get the list for the device type
    */
    struct arraylist* lst = devicetypes_get_devicelist(dev->devicetype);
    ASSERT_NOT_NULL(lst);
    /*
    * find the device
    */
    for (uint32_t i = 0; i < arraylist_count(lst); i++) {
        struct object* d = (struct object*)arraylist_get(lst, i);
        ASSERT_NOT_NULL(d);
        if (0 == strcmp(d->name, dev->name)) {
            /*
            * remove the device
            */
            arraylist_remove(lst, i);
            return;
        }
    }
}

uint16_t deviceregistry_devicecount() {
    uint16_t ret = 0;
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        struct arraylist* lst = devicetypes_get_devicelist(i);
        if (0 != lst) {
            ret = ret + arraylist_count(lst);
        }
    }
    return ret;
}

uint16_t deviceregistry_devicecount_type(device_type dt) {
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            return arraylist_count(lst);
        }
        return 0;
    } else {
        PANIC("Invalid device type passed to deviceregistry_devicecount_type");
    }
    return 0;
}

struct object* deviceregistry_get_device(device_type dt, uint16_t idx) {
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            return arraylist_get(lst, idx);
        } else {
            PANIC("there are no devices of type");
        }
    } else {
        PANIC("Invalid device type passed to deviceregistry_get_device");
    }
    return 0;
}

void deviceregistry_iterate(device_iterator deviceIterator) {
    if (0 != deviceIterator) {
        for (uint32_t i = 0; i < MAX_DEVICE_TYPES; i++) {
            struct arraylist* lst = devicetypes_get_devicelist(i);
            if (0 != lst) {
                for (uint32_t j = 0; j < arraylist_count(lst); j++) {
                    struct object* dev = (struct object*)arraylist_get(lst, j);
                    if (0 != dev) {
                        (*deviceIterator)(dev);
                    } else {
                        PANIC("null dev in deviceregistry_iterate");
                    }
                }
            }
        }
    } else {
        PANIC("Invalid iterator passed to deviceregistry_iterate");
    }
}

void deviceregistry_iterate_type(device_type dt, device_iterator deviceIterator) {
    ASSERT_NOT_NULL(deviceIterator);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* dev = (struct object*)arraylist_get(lst, j);
                if (0 != dev) {
                    (*deviceIterator)(dev);
                } else {
                    PANIC("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to deviceregistry_iterate");
    }
}

void deviceregistry_find_devices_by_description(device_type dt, const int8_t* description, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(description);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* dev = (struct object*)arraylist_get(lst, j);
                if (0 != dev) {
                    if (strcmp(dev->description, description) == 0) {
                        (*cb)(dev);
                    }
                } else {
                    PANIC("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to objectmgr_find_devices_by_description");
    }
}

void deviceregistry_find_devices_by_devicetype(device_type dt, deviceSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    if ((dt >= 0) && (dt < MAX_DEVICE_TYPES)) {
        struct arraylist* lst = devicetypes_get_devicelist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* dev = (struct object*)arraylist_get(lst, j);
                if (0 != dev) {
                    (*cb)(dev);
                } else {
                    PANIC("null dev in deviceregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to objectmgr_find_devices_by_description");
    }
}

/*
 * find device by name.  return zero if there is no such device
 */
struct object* deviceregistry_find_device(const int8_t* name) {
    ASSERT_NOT_NULL(name);
    for (uint16_t i = 0; i < MAX_DEVICE_TYPES; i++) {
        struct arraylist* lst = devicetypes_get_devicelist(i);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* dev = (struct object*)arraylist_get(lst, j);
                if (0 != dev) {
                    if (0 == strcmp(name, dev->name)) {
                        return dev;
                    }
                } else {
                    PANIC("null dev in deviceregistry_find_device");
                }
            }
        }
    }
    return 0;
}
