
# CosmOS Release roadmap

## CosmOS Release One (R1)

### Introduction

CosmOS R1 will be able to boot from disk, read an init process in userland, and fork processes.

CosmOS R1 will have 3 major features

1. FS Support
2. Multitasking
3. Userland

### FS Support

CosmOS R1 will support one filesystem that it can read and write from.  It will use this filesystem to read the initial user-land binary (the init process)

### Multitasking

CosmOS R1 will support multitasking and a user-land fork() API

### Userland

CosmOS R1 will support the jump to userland, kernel API's to read and write from boot disk, kernel APIs to read and write to serial, and to fork additional processes.

| Task                 | Developer  | Status
|----------------------|------------|--------------------------------|
| initrd FS Support    | Tom        | Started                        |
| Multitasking         | Kurt       | Not Started                    |
| Userland Jump        | Kurt       | Not Started                    |
| Flat binary Support  | Kurt       | Not Started                    |
| Loader               | ?          | Not Started                    |
| Syscalls             | Tom        | Requires Userland Jump         |
| init program         | Tom        | Not Started                    |


## CosmOS Release Two (R2)

CosmOS R2 will add prototypical TCP/IP networking.  

### Network Support

CosmOS R1 will support one network card (potentially virtio) and enough of the TCP/IP stack to implement ARP, ICMP and DHCP

| Task          | Developer   | Status
|-----------------------------|------------|---------------------|
| Virtio NIC                  | Jeff       | Started             |
| Virtio Disk                 | Jeff       | Started             |
| ARP                         | ?          | Not Started         |
| ICMP                        | ?          | Not Started         |
| DHCP                        | ?          | Not Started         |
| Relocatable Binary Support  | Kurt       | Not Started         |
| ELF Support                 | Kurt       | Not Started         |


## CosmOS Release Threw (R3)

CosmOS R3 will add initial clustering support including the ability to communicate between hosts and sync the VFS.

