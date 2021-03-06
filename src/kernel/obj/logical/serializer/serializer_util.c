//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************
#include <obj/logical/serializer/serializer.h>
#include <obj/logical/serializer/serializer_util.h>
#include <sys/debug/assert.h>
#include <sys/obj/object/object.h>
#include <sys/obj/objectinterface/objectinterface_serializer.h>

void serialize(struct object* serializer_object, struct object* obj, uint8_t* buffer, uint32_t size) {
    ASSERT_NOT_NULL(serializer_object);
    ASSERT_NOT_NULL(serializer_object->api);
    ASSERT_NOT_NULL(serializer_object->serialize);
    ASSERT_NOT_NULL(serializer_object->deserialize);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(size);
    struct objectinterface_serializer* api = (struct objectinterface_serializer*)serializer_object->api;
    (api->serialize)(obj, buffer, size);
}

void deserialize(struct object* serializer_object, struct object* obj, uint8_t* buffer, uint32_t size) {
    ASSERT_NOT_NULL(serializer_object);
    ASSERT_NOT_NULL(serializer_object->api);
    ASSERT_NOT_NULL(serializer_object->serialize);
    ASSERT_NOT_NULL(serializer_object->deserialize);
    ASSERT_NOT_NULL(obj);
    ASSERT_NOT_NULL(buffer);
    ASSERT_NOT_NULL(size);
    struct objectinterface_serializer* api = (struct objectinterface_serializer*)serializer_object->api;
    (api->deserialize)(obj, buffer, size);
}
