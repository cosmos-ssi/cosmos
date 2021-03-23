include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

SRC_DIR=.

all: $(APPNAME).bin

$(APPNAME).o: $(APPNAME).c
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o $(APPNAME).o $(APPNAME).c

$(APPNAME).elf: $(APPNAME).o
	$(LD) -m elf_x86_64 -o $(APPNAME).elf $(APPNAME).o ../../lib/abi/syscall.o ../../lib/abi/abi.o
	
$(APPNAME).bin: $(APPNAME).elf
	$(OBJCOPY) -O binary $(APPNAME).elf $(APPNAME).bin  

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) *.elf
