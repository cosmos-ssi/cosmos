//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/logical/fs/vfs/vfs.h>
#include <sys/debug/assert.h>
#include <sys/deviceapi/deviceapi_filesystem.h>
#include <sys/devicemgr/device.h>
#include <sys/fs/fs_helper.h>

struct filesystem_node* fshelper_get_fs_node(struct device* filesystem_device) {
    ASSERT_NOT_NULL(filesystem_device);
    ASSERT_NOT_NULL(filesystem_device->device_data);
    struct deviceapi_filesystem* fs_api = (struct deviceapi_filesystem*)filesystem_device->api;
    ASSERT_NOT_NULL(fs_api->root);
    return (*fs_api->root)(filesystem_device);
}

// void fshelper_add_device_to_vfs(struct device* vfs_device, struct device* dev) {
//     ASSERT_NOT_NULL(vfs_device);
//     ASSERT_NOT_NULL(vfs_device->api);
//     ASSERT_NOT_NULL(dev);
//     ASSERT_NOT_NULL(dev->api);

// vfs_add_child(vfs_device, )

//     if (dev->devicetype == FILESYSTEM) {
//         vfs_add_child
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_FS_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     } else {
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_DEV_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     }
// }

// void fshelper_remove_device_from_vfs(struct device* vfs_device, struct device* dev) {
//     ASSERT_NOT_NULL(vfs_device);
//     ASSERT_NOT_NULL(vfs_device->api);
//     ASSERT_NOT_NULL(dev);
//     ASSERT_NOT_NULL(dev->api);
// }

/*
* add device to VFS
*/
// void fshelper_add__device_to_vfs(struct device* dev) {
//     ASSERT_NOT_NULL(dev);
//     ASSERT_NOT_NULL(dev->api);
//     /*
//     * find the parent node where we want to add this
//     */
//     struct vfs_node* vfs_parent = 0;
//     if (dev->devicetype == FILESYSTEM) {
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_FS_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     } else {
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_DEV_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     }
//     /*
//     * find root fs_node of the filesystem
//     */
//     struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)dev->api;
//     struct filesystem_node* fs_root_node = (api->root)(dev);
//     ASSERT_NOT_NULL(fs_root_node);
//     /*
//     * make a vfs node
//     */
//     struct vfs_node* filesystem_vfs_node = vfsnode_new_node(fs_root_node);
//     /*
//     * add to vfs
//     */
//     vfsnode_add_child(vfs_parent, filesystem_vfs_node);
// }

// /*
// * remove device from VFS
// */
// void fshelper_remove_device_from_vfs(struct device* dev) {
//     ASSERT_NOT_NULL(dev);
//     ASSERT_NOT_NULL(dev->api);
//     /*
//     * find the parent node where we want to remove this
//     */
//     struct vfs_node* vfs_parent = 0;
//     if (dev->devicetype == FILESYSTEM) {
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_FS_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     } else {
//         vfs_parent = vfsnode_find_node_by_name(cosmos_vfs, VFS_DEV_TREE);
//         ASSERT_NOT_NULL(vfs_parent);
//     }
//     /*
//     * find root fs_node of the filesystem
//     */
//     struct deviceapi_filesystem* api = (struct deviceapi_filesystem*)dev->api;
//     struct filesystem_node* fs_root_node = (api->root)(dev);
//     ASSERT_NOT_NULL(fs_root_node);
//     /*
//     * remove
//     */
//     vfsnode_remove_child(vfs_parent, fs_root_node->id);
// }

// struct vfs_node* cosmos_vfs = 0;

// void vfs_init() {
//     /*
//     * get the root filesystem
//     */
//     struct device* root_fs_device = devicemgr_find_device("rootfs0");
//     ASSERT_NOT_NULL(root_fs_device);
//     /*
//     * get the root node of the root filesystem
//     */
//     struct deviceapi_filesystem* root_fs_api = (struct deviceapi_filesystem*)root_fs_device->api;
//     struct filesystem_node* fs_root_node = (root_fs_api->root)(root_fs_device);
//     ASSERT_NOT_NULL(fs_root_node);
//     /*
//     * get the root node of the root filesystem
//     */
//     cosmos_vfs = vfsnode_new_node(fs_root_node);
//     /*
//     * get the dev filesystem
//     */
//     struct device* root_devfs_device = devicemgr_find_device("devfs0");
//     ASSERT_NOT_NULL(root_devfs_device);
//     /*
//     * get the root node of the devfs filesystem
//     */
//     struct deviceapi_filesystem* root_devfs_api = (struct deviceapi_filesystem*)root_devfs_device->api;
//     struct filesystem_node* devfs_root_node = (root_devfs_api->root)(root_devfs_device);
//     ASSERT_NOT_NULL(devfs_root_node);
//     /*
//     * wrap the fs node in a vfs node
//     */
//     struct vfs_node* devfs_vfs_node = vfsnode_new_node(devfs_root_node);
//     /*
//     * add the vfs node to the root
//     */
//     vfsnode_add_child(cosmos_vfs, devfs_vfs_node);
// }

// void vfs_traverse_internal(struct vfs_node* v, vfs_traverse_function f, uint32_t depth) {
//     ASSERT_NOT_NULL(v);
//     ASSERT_NOT_NULL(f);
//     // call the callback
//     (*f)(v, depth);

//     // children
//     if (0 != v->children) {
//         for (uint32_t i = 0; i < arraylist_count(v->children); i++) {
//             struct vfs_node* c = (struct vfs_node*)arraylist_get(v->children, i);
//             vfs_traverse_internal(c, f, depth + 1);
//         }
//     }
// }

// /*
// * traverse
// */
// void vfs_traverse(struct vfs_node* v, vfs_traverse_function f) {
//     ASSERT_NOT_NULL(v);
//     ASSERT_NOT_NULL(f);
//     vfs_traverse_internal(v, f, 0);
// }

// void vfs_dump_traverse_function(struct vfs_node* v, uint32_t depth) {
//     ASSERT_NOT_NULL(v);
//     ASSERT_NOT_NULL(v->fs_node);

//     for (int32_t i = 0; i < depth; i++) {
//         kprintf(" ");
//     }
//     kprintf("%s %llu\n", v->fs_node->name, v->fs_node->id);
// }

// void vfs_dump(struct vfs_node* v) {
//     ASSERT_NOT_NULL(v);
//     vfs_traverse(v, &vfs_dump_traverse_function);
// }

// uint32_t vfs_count(struct vfs_node* v) {
//     ASSERT_NOT_NULL(v);
//     if (0 == v->children) {
//         return 0;
//     } else {
//         return arraylist_count(v->children);
//     }
// }
