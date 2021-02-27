//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _FILESYSTEM_DEVICE_HPP
#define _FILESYSTEM_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

#define FILESYSTEM_MAX_NAME 128
#define FILESYSTEM_MAX_FILES_PER_DIR 1024

enum fs_node_type { folder, file, device };

struct fs_node {
    /*
    * type
    */
    enum fs_node_type type;
    /* 
    * owning device
    */
    struct device* filesystem_device;
    /*
    * id
    */
    uint64_t id;
    /*
    * name
    */
    uint8_t name[FILESYSTEM_MAX_NAME];
    /*
    * node_specific data
    */
    void* node_data;
};

struct fs_directory {
    uint64_t count;
    uint64_t ids[FILESYSTEM_MAX_FILES_PER_DIR];
};

class FilesystemDevice : public Device {
  public:
    /*
    * get root node
    */
    struct fs_node* get_root_node(FilesystemDevice* filesystem_device);

    /*
    * read bytes from node
    */
    uint32_t read(struct fs_node* node, uint8_t* data, uint32_t data_size);
    /*
    * write bytes to node
    */
    uint32_t write(struct fs_node* node, const uint8_t* data, uint32_t data_size);
    void open(struct fs_node* node);
    void close(struct fs_node* node);
    /*
    * find a node by id
    */
    struct fs_node* find_node_by_id(struct fs_node* node, uint64_t id);
    /*
    * get directory list.  fills struct. 
    */
    void list_directory(struct fs_node* node, struct filesystem_directory* dir);
    /*
    * get file size 
    */
    uint64_t size(struct fs_node* node);
};

#endif