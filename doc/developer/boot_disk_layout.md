
# CosmOS boot disk

The cosmos boot has this layout


| Sector             | File  		 | Description
|--------------------|------------|---------------------------|
| 0                  | boot.bin   | Initial boot loader       |
| 1-2                | boot2.bin  | Stage 2 boot loader       |
| 3                  | boot3.bin  | Stage 3 boot loader       |
| 4-13 (10 sectors)  | initrd.img | initrd initial boot image |
| 14+                | cosmos.bin | Kernel                    |


The sector to find the kernel at is in boot2.asm.  Additionally the 
main Makefile for Cosmos has to be changed if there is a need to move the kernel.  