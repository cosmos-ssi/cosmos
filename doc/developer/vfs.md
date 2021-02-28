
# CosmOS Virtual File System

CosmOS has a hierarchical file system, similar to POSIX. 

```java
struct objecttype_filesystem {
    filesystem_get_root_node_function root;
    filesystem_read_function read;
    filesystem_write_function write;
    filesystem_open_function open;
    filesystem_close_function close;
    filesystem_find_node_by_id_function find_id;
    filesystem_find_node_by_name_function find_name;
    filesystem_find_node_by_idx_function find_idx;
    filesystem_count_function count;
};
```

The API `filesystem_get_root_node_function` gets the root filesystem node of the filesystem device and every other API exposed by `deviceapi_filesystem` takes a `filesystem_node*` as a parameter.  The API for a `filesystem_node` is:

```java
struct filesystem_node {
    /* 
    * owning device
    */
    struct object* filesystem_device;
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
```

Each node has:

* A pointer to the device that it is part of
* A 64-bit id that identifies it on the device
* A name
* A pointer to device-specific data


Nodes can be folders, files, devices, etc.


