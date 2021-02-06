include ../scripts/mk/flags.mk
include ../scripts/mk/build.mk

COSMOS_ABI_ARCHIVE=cosmos.ar

EXTRA_CFLAGS= -I.

SRC_DIR=.

all: $(COSMOS_ABI_ARCHIVE)

# sources
SRCS_C=$(shell find $(SRC_DIR) -type f -iname '*.c')
HEADERS_C=$(shell find $(SRC_DIR) -type f -iname '*.h')

# object files
OBJS_C=$(subst $(SRC_DIR)/, , $(SRCS_C:.c=.o))
OBJS=$(OBJS_C) $(OBJS_ASM)

# set search paths
vpath %.c .
vpath %.h .

$(COSMOS_ABI_ARCHIVE): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# C files	
%.o: %.c $(HEADERS_C)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)
	$(RM) $(ARCHIVE_FILE)
	$(RM) $(KERNEL_ELF)
	$(RM) $(KERNEL_BIN)
	$(RM) $(KERNEL_MAP)
