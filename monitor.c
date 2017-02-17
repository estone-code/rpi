#include "monitor.h"

#include "peripherals.h"
#include "console.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_ARGS 10

extern u64 *exception_table_addr;

struct command {
	char *name;
	int (*fn)(int, char**);
};

static int command_help_fn(int argc, char **argv)
{
	console_write("\nCommands:\n\n");

	console_write("peek <base-addr-hex> [<word-count>=1]\n");
	console_write("  notes: word-count is decimal, can be negative\n\n");

	console_write("poke <dest-word-base-addr-hex> ");
	console_write("<new-word-value-hex>\n");
	console_write("  notes: no cache flushing or mem barriers executed.\n");
	console_write("         The entire 8-byte word is overwritten.\n\n");

	console_write("test_string\n");
	console_write("  notes: prints string hardcoded ");
	console_write("in the binary at hardcoded addr.\n\n");

	console_write("quit\n");
	console_write("  notes: Quits the kernel monitor.\n\n");

	return 0;
}
struct command command_help = { .name = "help", .fn = command_help_fn };

static int command_quit_fn(int argc, char **argv)
{
	return 1;
}
struct command command_quit = { .name = "quit", .fn = command_quit_fn };

static int command_test_string_fn(int argc, char **argv)
{
	char *test_string;
	test_string = "Test String.";
	console_printf("test_string start-addr: %x\n", test_string);
	console_printf("test_string value: %s\n", test_string);

	console_printf("table addr: %x\n", exception_table_addr);

	return 0;
}
struct command command_test_string = { .name = "test_string",
				       .fn = command_test_string_fn };

static int command_peek_fn(int argc, char **argv)
{
	u64 base, to_print;
	s32 words, cur;

	words = 1;
	if (argc < 2) {
		console_write("Bad arguments.\n");
		return 0;
	}
	sscanf(argv[1], "%x", &base);
	if (base % 8) {
		console_write("Base must be word-aligned (8-byte words).\n");
		return 0;
	}
	if (argc > 2) {
		sscanf(argv[2], "%d", &words);
	}
	if (!words) {
		console_write("Word count cannot be 0.\n");
		return 0;
	}

	console_write("                      ");
	console_write("[addr] | [high-byte] [val] [low-byte]\n");
	for (cur = 0; cur < abs(words); cur++) {
		if (words > 0) {
			to_print = base + (8 * (words - cur - 1));
		} else {
			to_print = base - (8 * cur);
		}
		if (to_print >= 0) {
			console_printf("%x | %x\n", to_print, *(u64 *)(to_print));
		}
	}
	console_write("                      ");
	console_write("[addr] | [high-byte] [val] [low-byte]\n");
	return 0;
}

struct command command_peek = { .name = "peek", .fn = command_peek_fn };

static int command_poke_fn(int argc, char **argv)
{
	u64 addr, val;

	if (argc < 3) {
		console_write("Bad arguments.\n");
		return 0;
	}
	sscanf(argv[1], "%x", &addr);
	if (addr % 8) {
		console_write("Addr must be word-aligned (8-byte words).\n");
		return 0;
	}
	sscanf(argv[2], "%x", &val);
	*(u64 *)addr = val;
	return 0;
}
struct command command_poke = { .name = "poke", .fn = command_poke_fn };

static struct command *commands[] = {
	&command_quit,
	&command_help,
	&command_peek,
	&command_poke,
	&command_test_string,
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

void monitor_init(void)
{
	console_init();
}

void monitor(void)
{
	char *line, *token;
	char *args[MAX_ARGS];
	int i;
	int (*command_fn)(int, char**);

	console_write("\f\n\n    ");
	console_write("**************** EEP 64 MONITOR V1 ****************");
	console_write("\n\nREADY. Type 'help' for help.\n\n");
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
				if (command_fn(i, args)) {
					/* user wants to quit the monitor. */
					break;
				}
			} else {
				console_write("Unknown Command.\n");
			}
		}
	}
	return;
}
