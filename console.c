#include "aux.h"
#include "console.h"
#include "stdarg.h"
#include "types.h"

#define READLINE_BUF_SIZE 128
static char readline_buf[READLINE_BUF_SIZE];

s32 console_init()
{
	aux_mu_init();
	return 0;
}

s32 console_write_char(char c)
{
	char with_term_char[2];
	with_term_char[0] = c;
	with_term_char[1] = 0;
	return console_write(with_term_char);
}

s32 console_write(char *string)
{
	for (; *string; string++) {
		aux_mu_tx_byte_busywait(*string);
		if (*string == 0x0A) { /* LF */
			aux_mu_tx_byte_busywait(0x0D); /* CR */
		} else if (*string == 0x0D) { /* CR */
			aux_mu_tx_byte_busywait(0x0A); /* LF */
		}
	}
	return 0;
}

char console_read_char()
{
	return aux_mu_rx_byte_busywait();
}

static s32 console_write_hex(u64 c)
{
	unsigned char nibble, to_print;
	int nibbles;

	console_write("0x__");
	for (nibbles = 16; nibbles > 0; nibbles--) {
		nibble = (0xF000000000000000 & c) >> 60;
		/* convert to ascii */
		to_print = nibble + (nibble < 10 ? 48 : 55);
		console_write_char(to_print);
		c <<= 4;

		if (nibbles == 9) {
			console_write_char('_');
		}
		if (nibbles % 2 && nibbles != 1) {
			console_write_char('_');
		}
	}
	return 0;
}

/*
 * Embarrasing but quick-to-write recursive implementation that also
 * uses decimal division.
 */
static s32 console_write_decimal(s32 decimal)
{
	if (decimal < 0) {
		console_write_char('-');
		decimal *= -1;
	}
	if (decimal < 10) {
		console_write_char(decimal + 48); /* convert to ascii */
	} else {
		console_write_decimal(decimal / 10);
		console_write_char((decimal % 10) + 48);
	}
	return 0;
}

s32 console_printf(const char *format, ...)
{
	va_list ap;
	const char *c;
	u8 in_escape;

	in_escape = 0;
	va_start(ap, format);
	for (c = format; *c; c++) {
		if (in_escape) {
			in_escape = 0;
			switch (*c) {
			case 's':
				console_write(va_arg(ap, char *));
				break;
			case 'x':
			case 'p':
				console_write_hex(va_arg(ap, u64));
				break;
			case 'd':
				console_write_decimal(va_arg(ap, s32));
				break;
			case '%':
				console_write_char('%');
				break;
			}
		} else {
			if ('%' == *c) {
				in_escape = 1;
			} else {
				console_write_char(*c);
			}
		}
	}
	va_end(ap);
	return 0;
}

char * console_readline(void)
{
	u32 i;
	unsigned char c;

	i = 0;
	while (1) {
		if (i >= READLINE_BUF_SIZE-1) {
			break;
		}
		c = console_read_char();
		/* echo on the console */
		console_write_char(c);
		if (c == 0x7F) { /* ascii delete */
			if (!i) {
				/*
				 * we're already at beginning of
				 * buffer, just drop this char
				 */
				continue;
			}
			i--;
			/*
			 * there might be better ways to do this or
			 * configure my console differently
			 */
			console_write_char('\b');
			console_write_char(' ');
			console_write_char('\b');
		} else if (c >= ' ') {
			/* ascii chars above 0x20 (space) are printable */
			readline_buf[i]	= c;
			i++;
		} else if (c == '\n' || c == '\r') {
			break;
		}
	}
	readline_buf[i] = '\0';
	return readline_buf;
}
