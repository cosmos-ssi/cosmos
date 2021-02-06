
# CosmOS Memory Map

Cosmos boots from boot.asm, at location 0x7C00.  The second stage boot loader, boot2.asm is at 0x7E00, and the third stage boot loader, boot loader, is at 0x8200.

At kernel load, physical memory is mapped as follows:

| Start      | End        | Size    | Description                       |
|------------|------------|---------|-----------------------------------|
| 0x00000000 | 0x001FFFFF | 2MB     | Identity mapped                   |
| 0x500      |            |         | INT 15h memory map                |
| 0x10000    | 0x0007FFFF | 448KB   | Early kernel page tables          |
| 0x00100000 | 0x0017FFFF | 512k    | IO Buffers, identity mapped.      |
| 0x00180000 | 0x001FFFFF | 512k    | Unused, identity mapped.          |
| 0x00200000 | 0x003FFFFF | 2MB     | ATA(PI) Busmaster DMA Buffers     |
| 0x00300000 | 0x00FFFFFF | 14MB    | Unused, accessible via direct map |
| 0x01000000 | 0x017FFFFF | 8MB     | Kernel stack			            |
| 0x01800000 | 0x01FFFFFF | 8MB     | Kernel heap + text.               |


*The identity mapped region 0x00000000 - 0x001FFFFF includes many page tables and system data, and should not be used for any other purposes without serious discussion and planning

*The kernel stack is mapped at 0xFFFFFFFFFF800000

*The kernel heap is mapped at 0xFFFF800000000000

*The first 2MB of RAM are memory mapped
