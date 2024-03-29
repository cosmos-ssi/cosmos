include ../../../mk/flags.mk
include ../../../mk/build.mk
include ../user.mk

SRC_DIR=.

all: $(APPNAME).bin

$(APPNAME).o: $(APPNAME).cpp
	$(CC) $(USER_CFLAGS) $(EXTRA_CFLAGS) -c -o $(APPNAME).o $(APPNAME).cpp

$(APPNAME).elf: $(APPNAME).o
	$(LD) -m elf_x86_64 -o $(APPNAME).elf $(APPNAME).o $(COSMOS_ABI_ARCHIVE) $(COSMOS_VIDEO_ARCHIVE)
	
$(APPNAME).bin: $(APPNAME).elf
	$(OBJCOPY) -O binary $(APPNAME).elf $(APPNAME).bin  

clean:
	$(RM) *.o
	$(RM) *.bin
	$(RM) *.elf
