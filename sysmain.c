#include "peripherals.h"
#include "console.h"
#include "string.h"

void sysmain(void)
{
	char *line, *token;

	console_init();
	console_write("\n\n*** Welcome! ***\n\n");
	while (1) {
		console_write("kernel$ ");
		line = console_readline();
		token = strtok(line, " \t");
		while (token) {
			console_printf("  ** token: [%s]\n", token);
			token = strtok(0, " \t");
		}
	}
	return;
}
