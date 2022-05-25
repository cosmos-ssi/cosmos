# Partition types and GUIDs

## GUID Reference
CosmOS Kernel Partition: 1A11B2F8-874E-201C-1374-22BDF9368896 

CosmOS Node Configuration Partition: 7A4BD4D2-2DAF-F35B-83B7-A136D31DFA5E

CosmOS Data Partition: 4CCE86EE-4C0E-E1C5-DC57-903309A1A2C9 

## Type Descriptions

### CosmOS Kernel Partition
The CosmOS Kernel Partition is a raw, unformatted partition that contains the second- and third-stage bootloaders and the kernel image, in immediate and uninterrupted succession with the second-stage bootloader in the first block of the partition, the third-stage bootloader in the second block, and the kernel image beginning in the third block.  The partition must be at least large enough to contain all of these elements.