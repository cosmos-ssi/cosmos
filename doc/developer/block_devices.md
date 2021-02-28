# CosmOS Block Devices and Filesystems

Cosmos defines devices for:

- Disk Controllers (ata0, ata1...)
- Disks (disk0, disk1,...)
- Parition tables (pt0, pt1.. etc)
- Partitions (part0, part1,...
- FileSystems (fat0, sfs0,...etc)

Each layer builds upon the next layer. When Cosmos detects an IDE disk controller, such as `ata0`, it then defines disks for each of

- Primary Master
- Primary Slave
- Secondary Master
- Secondary Slave

These will show up as `disk0`, `disk1`, `disk2`, `disk3`.

CosmOS doesn't automatically read the partition table and mount disks (yet). To mount a disk, we use the partition table device.

The below code creates a partition table device for an MBR partition table on disk0. Running this code will result in

- creating a pt device such as `pt0`, for the MBR partition table
- creating multiple part devices such as `part0`, `part1`, `part2` for each partition.

```java
	struct object* dsk = objectmgr_find_object("disk0");
	if (0 != dsk) {
		struct object* dev = mbr_pt_attach(dsk);
		struct objecttype_part_table* api = (struct objecttype_part_table*)dev->api;
		mbr_pt_detach(dev);
	} else {
		kprintf("Unable to find disk0\n", devicename);
	}
```

Now that CosmOS knows about the partitions, we can mount filesystems on them. This code creates a FAT device such as `fat0' on 'part0'.

```java
	struct object* part = objectmgr_find_object("part0");
	if (0 != part) {
		struct object* dev = fat_attach(part);
		fat_detach(part);
	} else {
		kprintf("Unable to find part0\n", devicename);
	}
```

The device API's that each device type expose are:

- ata (none yet)
- disk [Block Device](../../src/kernel/sys/objectytpe/objecttype_block.h)
- pt [Partition Table](../../src/kernel/sys/objectytpe/objecttype_part_table.h)
- part [Block Device](../../src/kernel/sys/objectytpe/objecttype_block.h)
- filesystem [File System](../../src/kernel/sys/objectytpe/objecttype_filesystem.h)
