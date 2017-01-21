INSTALLDIR=/var/lib/tftpboot/
TOOLCHAIN=aarch64-none-elf
AS=$(TOOLCHAIN)-as
ASFLAGS=
CC=$(TOOLCHAIN)-gcc
CFLAGS=-O2 -Wall -nostdlib -nostartfiles -ffreestanding
LD=$(TOOLCHAIN)-ld
OBJCOPY=$(TOOLCHAIN)-objcopy
RM=rm -f
CP=cp

COBJS=sysmain.o

.PHONY: clean all install

all: kernel.img

install: kernel.img config.txt
	$(CP) $^ $(INSTALLDIR)

clean:
	$(RM) $(COBJS) setup.o kernel.elf kernel.img

# lowercase '.s' suffix skips preprocessor.
setup.o: setup.s
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

kernel.elf: kernel.lds setup.o $(COBJS)
	$(LD) -T kernel.lds -o $@ setup.o $(COBJS)

# raspberrypi's bootloader requires this name
kernel.img: kernel.elf
	$(OBJCOPY) $< -O binary $@
