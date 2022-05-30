/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _API_OBJECT_H
#define _API_OBJECT_H

#include <api/interfaces/interfaces.h>

typedef enum api_object_class_t { API_OBJECT_CLASS_ATA, API_OBJECT_CLASS_BLOCK } api_object_class_t;

typedef struct api_object_t {
    api_object_class_t object_class;
    void* class_data;
    implemented_interfaces_list interfaces;
} api_object_t;

api_object_t* api_new_object(api_object_class_t object_class, void* class_data);

#endif