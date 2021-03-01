//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/collection/arraylist/arraylist.h>
#include <sys/debug/assert.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectregistry/objectregistry.h>
#include <sys/string/string.h>

/*
* list of instances.  make this a tree.
*/
struct arraylist* object_reg;

void objectregistry_init() {
    object_reg = arraylist_new();
}

/*
* register a device
*/
void objectregistry_registerobject(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->objectype);
    ASSERT_NOT_NULL(object_reg);
    /*
    * add to the list
    */
    arraylist_add(object_reg, obj);
}

/*
* unregister a device
*/
void objectregistry_unregisterobject(struct object* obj) {
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(obj->objectype);
    ASSERT_NOT_NULL(object_reg);
    /*
    * find the device
    */
    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* d = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(d);
        if (0 == strcmp(d->name, obj->name)) {
            /*
            * remove the device
            */
            arraylist_remove(object_reg, i);
            return;
        }
    }
}

uint32_t objectregistry_objectcount() {
    ASSERT_NOT_NULL(object_reg);
    return arraylist_count(object_reg);
}

uint32_t objectregistry_objectcount_type(uint16_t dt) {
    ASSERT_NOT_NULL(object_reg);
    uint32_t ret = 0;
    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->objectype == dt) {
            ret += 1;
        }
    }
    return ret;
}

struct object* objectregistry_get_object(uint16_t dt, uint16_t idx) {
    ASSERT_NOT_NULL(object_reg);
    uint32_t count = 0;
    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->objectype == dt) {
            if (count == idx) {
                return o;
            } else {
                count += 1;
            }
        }
    }
    return 0;
}

void objectregistry_iterate(object_iterator objectIterator) {
    ASSERT_NOT_NULL(object_reg);
    ASSERT_NOT_NULL(objectIterator);
    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        (*objectIterator)(o);
    }
}

void objectregistry_iterate_type(uint16_t dt, object_iterator objectIterator) {
    ASSERT_NOT_NULL(object_reg);
    ASSERT_NOT_NULL(objectIterator);
    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->objectype == dt) {
            (*objectIterator)(o);
        }
    }
}

void objectregistry_find_objects_by_description(uint16_t dt, const int8_t* description, objectSearchCallback cb) {
    ASSERT_NOT_NULL(object_reg);
    ASSERT_NOT_NULL(cb);
    ASSERT_NOT_NULL(description);

    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->objectype == dt) {
            if (strcmp(o->description, description) == 0) {
                (*cb)(o);
            }
        }
    }
}

void objectregistry_find_objects_by_objectype(uint16_t dt, objectSearchCallback cb) {
    ASSERT_NOT_NULL(object_reg);
    ASSERT_NOT_NULL(cb);

    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->objectype == dt) {
            (*cb)(o);
        }
    }
}

struct object* objectregistry_find_object_by_name(const int8_t* name) {
    ASSERT_NOT_NULL(object_reg);

    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (strcmp(o->name, name) == 0) {
            return o;
        }
    }
    return 0;
}

struct object* objectregistry_find_object_by_handle(uint64_t handle) {
    ASSERT_NOT_NULL(object_reg);

    for (uint32_t i = 0; i < arraylist_count(object_reg); i++) {
        struct object* o = (struct object*)arraylist_get(object_reg, i);
        ASSERT_NOT_NULL(o);
        if (o->handle == handle) {
            return o;
        }
    }
    return 0;
}
