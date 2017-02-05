#include "peripherals.h"
#include "console.h"

void sysmain(void)
{
	char *line;

	console_init();
	console_write("\n\n*** Welcome! ***\n\n");
	while (1) {
		console_write("kernel$ ");
		line = console_readline();
		console_printf("echo: %s\n", line);
	}
	return;
}
