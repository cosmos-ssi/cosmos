/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <api/api_object.h>
#include <api/interfaces/interfaces.h>
#include <sys/kmalloc/kmalloc.h>

api_object_t** api_objects = 0;
uint64_t api_objects_count = 0;

api_object_t* api_new_object(api_object_class_t object_class, void* class_data) {
    api_objects = krealloc(api_objects, sizeof(api_object_t*) * (api_objects_count + 1));

    ASSERT_NOT_NULL(api_objects);

    api_objects[api_objects_count] = kmalloc(sizeof(api_object_t));

    ASSERT_NOT_NULL(api_objects[api_objects_count]);

    api_objects[api_objects_count]->object_class = object_class;
    api_objects[api_objects_count]->class_data = class_data;

    api_objects_count++;

    return api_objects[api_objects_count - 1];
}