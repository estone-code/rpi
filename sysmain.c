#include "monitor.h"
#include "types.h"

/*
 * Obtain values with '&'
 * https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html#Source-Code-Reference
 */
extern unsigned long _START_OF_BSS;
extern unsigned long _END_OF_BSS;

void sysmain(void)
{
	uintptr_t *ptr;

	/* init .bss */
	for (ptr = &_START_OF_BSS; ptr < &_END_OF_BSS; ptr++) {
		*ptr = 0;
	}

	monitor_init();
	while (1) {
		monitor();
	}
}
