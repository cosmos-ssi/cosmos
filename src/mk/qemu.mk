#*****************************************************************
# This file is part of CosmOS                                    *
# Copyright (C) 2020-2021 Tom Everett                            *
# Released under the stated terms in the file LICENSE            *
# See the file "LICENSE" in the source distribution for details  *
# ****************************************************************

# qemu
QEMU=qemu-system-x86_64

# debug args
QEMUDEBUGARGS=                                            \
  -gdb tcp::1234                                          \
  -S

# args
QEMUARGS=                                                 \
  -accel tcg,thread=single                                \
  -cpu core2duo                                           \
  -m 4096                                                \
  -smp 1                                                  \
  -drive file=../build/src/img/hda/hda.img,index=0,format=raw              \
  -drive file=img/root.img,index=1,format=raw           \
  -drive file=img/gpt_fat.img,index=2,format=raw          \
  -device sdhci-pci                                     \
  -device virtio-net-pci,netdev=net0                   \
  -netdev user,id=net0,hostfwd=tcp::8080-:80             \
  -object filter-dump,id=f1,netdev=net0,file=net0.dat     \
  -serial stdio                                           \
  -audiodev coreaudio,id=audio0                           \
  -device adlib,audiodev=audio0                          \
  -monitor telnet::45454,server,nowait                    \
  -D qemu.log                                               \
  -d guest_errors,trace:*net*,trace:*virtio*,trace:*eth*,trace:*pci*,trace:*ioapic*   \
  -vga std

  # -drive file=img/hda.img,index=0,format=raw              \
  # -drive file=../build/src/img/hda/hda.img,index=0,format=raw              \

#   -drive file=img/root.img,index=3,format=raw          \

  # -object filter-dump,id=f1,netdev=virtio,file=dump.dat      \
 #  -device usb-ehci                                        \

#  -drive if=virtio,file=img/hda.img,format=raw                \
#  -device sb16,audiodev=audio0                            \
# -nic user,model=virtio-net-pci                          \
  -netdev user,id=n0,hostfwd=tcp::8080-:80  \
 
#   -nic user,model=rtl8139,net=192.168.76.0/24,dhcpstart=192.168.76.9,id=net1 \


# note that we're mounting the img/hda.img as a floppy image. good enough for now.
# the first bytes are FA B4 00 B0 03 CD 10 88 16 7C 7C B8 01 24 CD 15

# to list devices:
# qemu-system-x86_64 -device ?

# -device AC97,audiodev=audio0
# -device adlib,audiodev=audio0

#  -machine pcspk-audiodev=audio0                          \

# -device floppy,drive=f0                                 \
#  -blockdev driver=file,node-name=f0,filename=img/hda.img     \
 
# -device sdhci-pci                                       \
#  -blockdev driver=file,node-name=sddrive,filename=img/hda.img \
#  -device sd-card                                                   
