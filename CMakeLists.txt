cmake_minimum_required(VERSION 3.16)
project(
  CosmOS
  VERSION 1.0
  LANGUAGES C CXX ASM_NASM
)

option(QEMU_USE_GDB "Run QEMU with the GDB server enabled. Waits for a connection before booting." OFF)

# set dialects
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# build utilities for the build process itself
add_subdirectory(src/util/mkinitrd)

# build a bootimage binary for TARGET_PLATFORM
add_subdirectory(src/boot)

# build a kernel binary for TARGET_PLATFORM
add_subdirectory(src/kernel)

# build an initial ramdisk (initrd) binary
add_subdirectory(src/img/initrd)

# target: hda Compose the bootimage, kernel and initrd into a bootable disk image that qemu can load
add_subdirectory(src/img/hda)

# set up convenience target to boot in qemu (if available)
find_program(QEMU_BIN qemu-system-x86_64)
if(QEMU_BIN)
  # cmake-format: off
  set (QEMU_ARGS 
    -accel tcg,thread=single                               
    -cpu core2duo                                           
    -m 4096                                                
    -smp 1                                                  
    -drive file=hda.img,index=0,format=raw              
    -drive file=../src/img/root.img,index=1,format=raw           
    -drive file=../src/img/gpt_fat.img,index=2,format=raw          
    -device sdhci-pci                                     
    -device virtio-net-pci,netdev=net0                   
    -netdev user,id=net0,hostfwd=tcp::8080-:80             
    -object filter-dump,id=f1,netdev=net0,file=net0.dat     
    -serial stdio                                           
    -audiodev coreaudio,id=audio0                           
    -device adlib,audiodev=audio0                          
    -monitor telnet::45454,server,nowait                    
    -D qemu.log                                               
    -d guest_errors,trace:*net*,trace:*virtio*,trace:*eth*,trace:*pci*,trace:*ioapic*   
    -vga std
    $<$<BOOL:${QEMU_USE_GDB}>:-gdb>
    $<$<BOOL:${QEMU_USE_GDB}>:tcp::1234>
    $<$<BOOL:${QEMU_USE_GDB}>:-S>
  )
  # cmake-format: on

  add_custom_target(
    qemu
    COMMAND ${QEMU_BIN} ${QEMU_ARGS}
    BYPRODUCTS qemu.log net0.dat
  )
  add_dependencies(qemu hda)
endif()
