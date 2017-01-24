#include "system.h"
#include "types.h"

/*
 * This spins for /at least/ the input number of cycles.
 */
void delay(u32 cycles)
{
	u32 count;
	for (count = 0; count < cycles; count++) {
		noop();
	}
}
