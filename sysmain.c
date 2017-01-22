#include "peripherals.h"

void sysmain(void)
{
	/*
	 * Note that between the pi1/2 and pi3, I think the GPIO pin
	 * assignments for mini-UART and full-UART were switched. This
	 * is because bluetooth on pi3 uses the full UART. So pin
	 * 14/15 on ALT_FUNC_0 are the mini-uart which is different
	 * from a lot of online documentation.
	 *
	 * The sample code I'm working from sets up the UART first then
	 * changes the GPIO pins, but I'm going to try reversing that.
	 */

	/*
	 * set up the mini-UART's GPIO pins
	 * 1. set the pins "alternatve functions"
	 * 2. Disable the default "pull" on the pins (don't want high OR low)
	 *    This is done with a careful procedure detailed in the BCM doc
	 *    that involves:
	 *    - set a GPIO control register with new pull values
	 *    - wait 150 cycles (to let the value get into the device?)
	 *    - set a different GPIO control register that tells our specific
	 *      pins to change their pull value to be what's now in that
	 *      first control register
	 *    - wait 150 cycles for the second control instructin to get to
	 *      the device and take effect
	 *    - reset the 1st control register that was modified to a default
	 *      value
	 *    - reset the 2nd control register modified to a default value
	 */

	/* set GPFSEL1 to have both pin 14 and 15 use ALT_FUNC_0 */

	/*
	 * 'enable' the mini-uart. It will start receiving signals
	 * from the pins immediately. Now that its control registers
	 * are active, set them to the settings we want.
	 *
	 * The sample code I'm looking at says the datasheet is wrong about
	 * some control-bit settings so watch out for that.
	 *
	 */

	return;
}
