cmake_minimum_required(VERSION 3.16)
project(CosmOS VERSION 1.0)

set(TARGET_PLATFORM "x86-64")

# target: bootimage Build a TARGET_PLATFORM specific bootimage binary
add_subdirectory(src/boot)

# target: kernel Build a TARGET_PLATFORM specific kernel binary
add_subdirectory(src/kernel)

# target: initrd Build an initial ramdisk (initrd) as a binary
add_subdirectory(src/util/mkinitrd)

# target: hda Compose the bootimage, kernel and initrd into a bootable disk image that qemu can load
add_subdirectory(src/img/hda)
