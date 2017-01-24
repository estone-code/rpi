#include "aux.h"
#include "console.h"
#include "types.h"

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

static char nibble_to_char(unsigned char c)
{
	if (c < 10) {
		return c+48;
	}
	return c+55;
}

s32 console_write_hex(unsigned char c)
{
	unsigned char nibble, to_print;

	nibble = (c >> 4) & 0xF;
	to_print = nibble_to_char(nibble);
	console_write_char(to_print);
	nibble = c & 0xF;
	to_print = nibble_to_char(nibble);
	console_write_char(to_print);
	return 0;
}
