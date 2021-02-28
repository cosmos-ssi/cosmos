
# Working with filesystems

Cosmos include "fsfacade.h" to enable easily working with file systems.

The first step of working with a filesystem is to get a filesystem mounted on a disk.

This little piece of code mounts an `initrd` filesystem on the device `disk0`.  It's important to note that the device `struct object* initrd` implements `deviceapi_filesystem`.


```java
struct object* dsk = objectmgr_find_device("disk0");
if (0 != dsk) {
	struct object* initrd = initrd_attach(dsk, initrd_lba());
	ASSERT_NOT_NULL(initrd);
```

Example devices which implement `deviceapi_filesystem` include

* vfs
* initrd
* devfs
* fat
* sfs
* tfs
* cfs

The functions in fsfacade.h operate on filesystem nodes (`struct filesystem_node*`) therefore we need to get the root node of the device.

```java
// get root node
struct filesystem_node* fsfacade_get_fs_rootnode(initrd);
```

Once we have the root node, we can use the other functions in `fsfacade.h`:

```java
struct filesystem_node* fsfacade_get_fs_rootnode(struct object* filesystem_device);

typedef void (*fsfacade_traverse_function)(struct filesystem_node* fs_node, uint32_t depth);

void fsfacade_traverse(struct filesystem_node* fs_node, fsfacade_traverse_function f);
void fsfacade_dump(struct filesystem_node* fs_node);

void fsfacade_list_directory(struct filesystem_node* fs_node, struct filesystem_directory* dir);

struct filesystem_node* fsfacade_find_node_by_id(struct filesystem_node* fs_node, uint32_t id);

uint64_t fsfacade_size(struct filesystem_node* fs_node);

struct filesystem_node* fsfacade_find_file_by_name(struct filesystem_node* fs_node, char* name);

uint32_t fsfacade_read(struct filesystem_node* fs_node, uint8_t* data, uint32_t data_size);

uint32_t fsfacade_write(struct filesystem_node* fs_node, const uint8_t* data, uint32_t data_size);

```