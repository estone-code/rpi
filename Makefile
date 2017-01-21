# Builds .o's from .s and .c files and links them into an ELF binary.
# (.S uppercase files are not supported by this makefile so assembler
# preproccessor will not run)
# A specific .o can be specified to be first in the link-order.
# Then translates the ELF to a plain "binary binary". Eventually
# would like to keep ELF for the debug symbols. This will require
# some more knowledge of ldscript-ing, where to set the initial PC,
# how to determine size of the loaded file etc.

INSTALLDIR := /var/lib/tftpboot/
TOOLCHAIN := aarch64-none-elf
LDSCRIPT := kernel.lds
# the first (generated) object to be used in the link order. This code
# probably first needs to set up the system's stack pointer.
SETUPOBJ := setup.o

# keep compiler from adding any extra library or setup code
CCFLAGS := -O2 -Wall -nostdlib -nostartfiles -ffreestanding
ASFLAGS :=
LDFLAGS :=

# raspberrypi's bootloader requires these exact names, so unlikely to be
# changed. This Makefile expects the .img suffix in its logic.
BINFILE := kernel.img
CONFFILE := config.txt

AS := $(TOOLCHAIN)-as
CC := $(TOOLCHAIN)-gcc
LD := $(TOOLCHAIN)-ld
OBJCOPY := $(TOOLCHAIN)-objcopy

# sorting is for deterministic link order
SOBJS := $(sort $(filter-out $(SETUPOBJ),$(patsubst %.s,%.o,$(wildcard *.s))))
COBJS := $(sort $(filter-out $(SETUPOBJ),$(patsubst %.c,%.o,$(wildcard *.c))))
ELFFILE := $(BINFILE:.img=.elf)

.PHONY: build clean install validate
.DEFAULT_GLOBAL: build

build: validate $(BINFILE)

install: build
	cp $(BINFILE) $(CONFFILE) $(INSTALLDIR)

clean:
	rm -f *.o $(ELFFILE) $(BINFILE)

validate:
	@test 0 -eq $$(basename -s .c $$(basename -s .s $$(ls *.s *.c)) \
	        | sort | uniq -d | wc -l) \
	    || (echo 'Detected source files with identicial basenames.' \
	        && false)

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c $(CCFLAGS) -o $@ $<

$(LDSCRIPT):

$(ELFFILE): $(LDSCRIPT) $(SETUPOBJ) $(SOBJS) $(COBJS)
	$(LD) -T $(LDSCRIPT) $(LDFLAGS) -o $@ $(SETUPOBJ) $(SOBJS) $(COBJS)

$(BINFILE): $(ELFFILE)
	$(OBJCOPY) $< -O binary $@
