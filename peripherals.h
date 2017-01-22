#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "gpio.h"
#include "aux.h"

/*
 * The "I/O Base" address for the BCM2836 and 7 is 0x3F000000 . This
 * is an "ARM Physical Address". The ARM cores on the BCM2835
 * (...6,..7) don't actually "run" the SoC, that would be the
 * VideoCore4 GPU. VC4's 'virtual' view of addresses to RAM are called
 * "Bus Addresses" in the BCM2835 documentation. The VC4 uses a
 * seperate MMU from the ARM's MMU. The "bus address" mapping of RAM
 * can be completely ignored by us -except- when looking at BCM2835
 * peripheral documentation. The docs all give peripheral registers as
 * "Bus Addresses". Also, when issuing DMA the peripheral's bus
 * addresses are used.
 *
 * The "bus address" of IOBASE is 0x7EOOOOOO , while the "ARM physical
 * address" is 0x3F000000 . So when the docs say a peripheral address
 * is 0x7E010000 , on our ARM core that would be 0x3F010000.
 * Basically, change the "7E" prefix to a "3F".
 *
 * In the header files we list ARM physical addresses. Hopefully there
 * won't be too many errors in manually copying and adjusting the
 * addresses from the SoC's datasheet.
 *
 * 'size' is given in bits (not bytes).
 */

#endif /* PERIPHERALS_H */
