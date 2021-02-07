
# CosmOS boot disk

The cosmos boot has this layout


| Sector     | File  		 | Description
|------------|------------|---------------------------|
| 0          | boot.bin   | Initial boot loader       |
| 1-2        | boot2.bin  | Stage 2 boot loader       |
| 3          | boot3.bin  | Stage 3 boot loader       |
| 4+         | cosmos.bin | Kernel                    |
