#include "peripherals.h"
#include "console.h"

void sysmain(void)
{
	unsigned char c;

	console_init();
	console_write("\nWelcome!\n");
	while (1) {
		console_write("kernel$ ");
		while (1) {
			c = console_read_char();
			console_write_char(c);
			if (c == 0x0D) { /* CR */
				break;
			}
		}
	}
	return;
}
