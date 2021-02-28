include mk/build.mk

BOOTIMAGE=img/hda.img
BLANK_DISK=img/blank.img
INITRD=img/initrd.img

SRC_FILES=$(shell find . -type f \( -name "*.c" -o -name "*.h" \))

CRUFT_FILES=$(shell find . -type f \( -name "dump.dat" -o \
									  -name "qemu.log" \))

all: subsystems
.PHONY: clean header-check header-fix

bootimage:  lint subsystems blank-disk
	# make a file
	#$(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024
	test -f $(BOOTIMAGE) || $(DD) if=/dev/zero of=$(BOOTIMAGE) bs=32768 count=129024 

	# write boot loader
	$(DD) if=boot/x86-64/boot.bin of=$(BOOTIMAGE) conv=notrunc bs=512 
	$(DD) if=boot/x86-64/boot2.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=1
	$(DD) if=boot/x86-64/boot3.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=3

	# write kernel
	$(DD) if=kernel/cosmos.bin of=$(BOOTIMAGE) conv=notrunc bs=512 seek=4

	# write initrd fs at offset of 10MB
	$(DD) if=$(INITRD) of=$(BOOTIMAGE) conv=notrunc bs=512 seek=20480

subsystems: boot-subsystem kernel-subsystem utils user-subsystem
	
blank-disk:
	$(DD) if=/dev/zero of=$(BLANK_DISK) bs=1024 count=10240

boot-subsystem:
	cd boot/x86-64 && $(MAKE)
	
kernel-subsystem:
	cd kernel && $(MAKE) all

user-subsystem:
	cd user && $(MAKE) all

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

header-check:
	./util/iwyu_map_check.sh kernel kernel/iwyu_mapping.imp
	cd kernel && $(MAKE) -k "CC=iwyu -Xiwyu --mapping_file=iwyu_mapping.imp -resource-dir . -isystem. -I." all 2>&1 > /dev/null | \
	fix_include --noreorder --comments --nosafe_headers --dry_run

header-fix:
	./util/iwyu_map_check.sh kernel kernel/iwyu_mapping.imp
	cd kernel && $(MAKE) -k "CC=iwyu -Xiwyu --mapping_file=iwyu_mapping.imp -resource-dir . -isystem. -I." all 2>&1 > /dev/null | \
	fix_include --reorder --nocomments --nosafe_headers
