include ../scripts/mk/flags.mk
include ../scripts/mk/build.mk

COSMOS_ABI_ARCHIVE=cosmos_abi.a

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_ABI_ARCHIVE) init.o

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h')
SRCS_ASM=$(shell find $(SRC_DIR) -type f -iname '*.asm')

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS_ASM=$(subst $(SRC_DIR)/, , $(SRCS_ASM:.asm=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

# set search paths
vpath %.c .
vpath %.h .
vpath %.asm .

$(COSMOS_ABI_ARCHIVE): abi/posix/posix_abi.o abi/cosmos/cosmos_abi.o abi/bdos/bdos_abi.o
	$(AR) $(ARFLAGS) $(COSMOS_ABI_ARCHIVE) abi/posix/posix_abi.o abi/cosmos/cosmos_abi.o abi/bdos/bdos_abi.o

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

# asm files	
%.o: %.asm
	$(NASM) $(NASMARGS) $< -o $@ 
	
clean:
	$(RM) $(OBJS)
	$(RM) $(ARCHIVE_FILE)
	$(RM) $(KERNEL_ELF)
	$(RM) $(KERNEL_BIN)
	$(RM) $(KERNEL_MAP)
