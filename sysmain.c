#include "peripherals.h"
#include "console.h"
#include "string.h"

#define MAX_ARGS 10

struct command {
	char *name;
	int (*fn)(int, char**);
};

static int command_help_fn(int argc, char **argv)
{
	console_write("TODO: Implement help.\n");
	return 0;
}
struct command command_help = { .name = "help", .fn = command_help_fn };

static int command_peek_fn(int argc, char **argv)
{
	console_write("TODO: Implement peek.\n");
	return 0;
}
struct command command_peek = { .name = "peek", .fn = command_peek_fn };

static int command_poke_fn(int argc, char **argv)
{
	console_write("TODO: Implement poke.\n");
	return 0;
}
struct command command_poke = { .name = "poke", .fn = command_poke_fn };

static struct command *commands[] = {
	&command_help,
	&command_peek,
	&command_poke,
	0
};

/*
 * Returns NULL if not found.
 */
static int (*find_command(const char *command_name))(int, char**)
{
	struct command **cmd;
	for (cmd = commands; *cmd; cmd++) {
		if (!strcmp((*cmd)->name, command_name)) {
			return (*cmd)->fn;
		}
	}
	return 0;
}

void sysmain(void)
{
	char *line, *token;
	char *args[MAX_ARGS];
	int i;
	int (*command_fn)(int, char**);

	console_init();
	console_write("\n\n*** Welcome! ***\n\n");
	while (1) {
		console_write("kernel$ ");
		line = console_readline();
		i = 0;
		token = strtok(line, " \t");
		while (token && i < MAX_ARGS) {
			args[i] = token;
			i++;
			token = strtok(0, " \t");
		}
		args[i] = 0;
		if (i > 0) {
			command_fn = find_command(args[0]);
			if (command_fn) {
				command_fn(i, args);
			} else {
				console_write("Unknown Command.\n");
			}
		}
	}
	return;
}
