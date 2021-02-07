
# CosmOS boot disk

The cosmos boot has this layout


| Sector           | Offset     | File  	 | Description
|------------------|------------|------------|---------------------------|
| 0                |  0         | boot.bin   | Initial boot loader       |
| 1-2              |  512B      | boot2.bin  | Stage 2 boot loader       |
| 3                |  1.5K      | boot3.bin  | Stage 3 boot loader       |
| 4+               |  2k        | cosmos.bin | Kernel                    |
| 20480            |  10M       | initrd.img | initrd initial boot image |

The sector to find the kernel at is in boot2.asm.  Additionally the 
main Makefile for Cosmos has to be changed if there is a need to move the kernel. 

512B sectors are assumed
 