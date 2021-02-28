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
#include <sys/obj/objectinterface/objectinterface_filesystem.h>
#include <sys/obj/objectregistry/objectregistry.h>
#include <sys/obj/objecttypes/objecttypes.h>
#include <sys/string/string.h>

void objectregistry_init() {
    /*
    * init the types
    */
    objecttypes_init();
}

/*
* register a device
*/
void objectregistry_registerobject(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->objectype);

    /*
    * get the list for the device type
    */
    struct arraylist* lst = objecttypes_get_objectlist(obj->objectype);
    if (0 == lst) {
        lst = arraylist_new();
        objecttypes_set_objectlist(obj->objectype, lst);
    }
    /*
    * add to the list
    */
    arraylist_add(lst, obj);
}

/*
* unregister a device
*/
void objectregistry_unregisterobject(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->objectype);
    /*
    * get the list for the device type
    */
    struct arraylist* lst = objecttypes_get_objectlist(obj->objectype);
    ASSERT_NOT_NULL(lst);
    /*
    * find the device
    */
    for (uint32_t i = 0; i < arraylist_count(lst); i++) {
        struct object* d = (struct object*)arraylist_get(lst, i);
        ASSERT_NOT_NULL(d);
        if (0 == strcmp(d->name, obj->name)) {
            /*
            * remove the device
            */
            arraylist_remove(lst, i);
            return;
        }
    }
}

uint16_t objectregistry_objectcount() {
    uint16_t ret = 0;
    for (uint16_t i = 0; i < MAX_OBJECT_TYPES; i++) {
        struct arraylist* lst = objecttypes_get_objectlist(i);
        if (0 != lst) {
            ret = ret + arraylist_count(lst);
        }
    }
    return ret;
}

uint16_t objectregistry_objectcount_type(enum object_type_id dt) {
    if ((dt >= 0) && (dt < MAX_OBJECT_TYPES)) {
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            return arraylist_count(lst);
        }
        return 0;
    } else {
        PANIC("Invalid device type passed to objectregistry_objectcount_type");
    }
    return 0;
}

struct object* objectregistry_get_object(enum object_type_id dt, uint16_t idx) {
    if ((dt >= 0) && (dt < MAX_OBJECT_TYPES)) {
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            return arraylist_get(lst, idx);
        } else {
            PANIC("there are no devices of type");
        }
    } else {
        PANIC("Invalid device type passed to objectregistry_get_object");
    }
    return 0;
}

void objectregistry_iterate(object_iterator objectIterator) {
    if (0 != objectIterator) {
        for (uint32_t i = 0; i < MAX_OBJECT_TYPES; i++) {
            struct arraylist* lst = objecttypes_get_objectlist(i);
            if (0 != lst) {
                for (uint32_t j = 0; j < arraylist_count(lst); j++) {
                    struct object* obj = (struct object*)arraylist_get(lst, j);
                    if (0 != obj) {
                        (*objectIterator)(obj);
                    } else {
                        PANIC("null dev in objectregistry_iterate");
                    }
                }
            }
        }
    } else {
        PANIC("Invalid iterator passed to objectregistry_iterate");
    }
}

void objectregistry_iterate_type(enum object_type_id dt, object_iterator objectIterator) {
    ASSERT_NOT_NULL(objectIterator);
    if ((dt >= 0) && (dt < MAX_OBJECT_TYPES)) {
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* obj = (struct object*)arraylist_get(lst, j);
                if (0 != obj) {
                    (*objectIterator)(obj);
                } else {
                    PANIC("null dev in objectregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to objectregistry_iterate");
    }
}

void objectregistry_find_objects_by_description(enum object_type_id dt, const int8_t* description,
                                                objectSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(description);
    if ((dt >= 0) && (dt < MAX_OBJECT_TYPES)) {
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* obj = (struct object*)arraylist_get(lst, j);
                if (0 != obj) {
                    if (strcmp(obj->description, description) == 0) {
                        (*cb)(obj);
                    }
                } else {
                    PANIC("null dev in objectregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to objectmgr_find_objects_by_description");
    }
}

void objectregistry_find_objects_by_objectype(enum object_type_id dt, objectSearchCallback cb) {
    ASSERT_NOT_NULL(cb);
    if ((dt >= 0) && (dt < MAX_OBJECT_TYPES)) {
        struct arraylist* lst = objecttypes_get_objectlist(dt);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* obj = (struct object*)arraylist_get(lst, j);
                if (0 != obj) {
                    (*cb)(obj);
                } else {
                    PANIC("null dev in objectregistry_iterate");
                }
            }
        }
    } else {
        PANIC("Invalid device_type passed to objectmgr_find_objects_by_description");
    }
}

/*
 * find device by name.  return zero if there is no such device
 */
struct object* objectregistry_find_object(const int8_t* name) {
    ASSERT_NOT_NULL(name);
    for (uint16_t i = 0; i < MAX_OBJECT_TYPES; i++) {
        struct arraylist* lst = objecttypes_get_objectlist(i);
        if (0 != lst) {
            for (uint16_t j = 0; j < arraylist_count(lst); j++) {
                struct object* obj = (struct object*)arraylist_get(lst, j);
                if (0 != obj) {
                    if (0 == strcmp(name, obj->name)) {
                        return obj;
                    }
                } else {
                    PANIC("null dev in objectregistry_find_object");
                }
            }
        }
    }
    return 0;
}
