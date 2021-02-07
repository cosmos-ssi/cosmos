
# Using CosmOS block devices

Numerous CosmOS devices implement the API `deviceapi_block`.   For example:

* ata disks
* partitions
* ram disks
* virtio disks

CosmOS provides the utility functions in `blockutil.h` for interfacing with devices which implement `deviceapi_block`.  It is strongly recommended to use the utility functions rather than using `deviceapi_block` directly.  `blockutil` implements protections and checks to ensure that the API calls are reasonable and that the behaviour of the underlying block devices is reasonable.

Typically filesystem devices and partition table devices need to use the services of block devices: they always use `blockutil`.

Example code to read from block device:

```java
    uint32_t s = strlen(testdata);
    uint32_t written = blockutil_write(dev, testdata, s + 1, 0);
````

Example code to write from block devices.

```java
    uint8_t buffer[s + 1];
    uint32_t read = blockutil_read(dev, (uint8_t*)buffer, s + 1, 0);
```

