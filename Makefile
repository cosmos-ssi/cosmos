include scripts/mk/build.mk

BOOTIMAGE=img/hda.img
BLANK_DISK=img/blank.img
SRC_FILES=$(shell find . -type f \( -name "*.c" -o -name "*.h" \))

CRUFT_FILES=$(shell find . -type f \( -name "dump.dat" -o \
									  -name "qemu.log" \))

all: subsystems
.PHONY: clean

bootimage: subsystems
	# make a file
	$(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024

	# write boot loader
	$(DD) if=boot/x86-64/boot.bin of=$(BOOTIMAGE) conv=notrunc bs=512 
	$(DD) if=boot/x86-64/boot2.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=1
	$(DD) if=boot/x86-64/boot3.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=3

	# write kernel
	$(DD) if=kernel/cosmos.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=4

	# write initrd fs at offset of 10MB
	$(DD) if=img/initrd.img of=$(BOOTIMAGE) conv=notrunc bs=512 seek=20480

subsystems: lint boot-subsystem kernel-subsystem utils user-subsystem blank-disk initrd
	
blank-disk:
	$(DD) if=/dev/zero of=$(BLANK_DISK) bs=1024 count=10240

boot-subsystem:
	cd boot/x86-64 && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE) all

user-subsystem:
	cd user && $(MAKE) all

initrd:
	rm -f initrd.img
	./util/mkinitrd/mkinitrd user/init.o init.o user/abi/bdos/bdos_abi.a bdos_abi.a user/abi/posix/posix_abi.a posix_abi.a user/abi/cosmos/cosmos_abi.a cosmos_abi.a
	mv initrd.img img/

utils:
	cd util/mkinitrd && $(MAKE)

clean: boot-clean kernel-clean user-clean util-clean
	$(RM) $(BOOTIMAGE)
	$(RM) $(BLANK_DISK)
	$(RM) $(CRUFT_FILES)

kernel-clean:
	cd kernel && $(MAKE) clean

boot-clean:
	cd boot/x86-64 && $(MAKE) clean

user-clean:
	cd user && $(MAKE) clean

util-clean:
	cd util/mkinitrd && $(MAKE) clean

qemu: bootimage
	$(QEMU) $(QEMUARGS)

qemu-debug: bootimage
	$(QEMU) $(QEMUARGS) $(QEMUDEBUGARGS)

lint:
	clang-format -n --Werror -style=file $(SRC_FILES)