#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

s32 console_init(void);
s32 console_printf(const char *format, ...);
s32 console_write(char *string);
s32 console_write_char(char);
char console_read_char(void);

/*
 * Echoes characters back to the sender as they are typed.  Returns to
 * the caller once with buffered input after a newline is received.
 *
 * Re-uses the same internal buffer between invocations, so copy
 * contents into your own buffer if calling this again.
 */
char * console_readline(void);

#endif /* CONSOLE_H */
