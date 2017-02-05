#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

s32 console_init(void);
s32 console_printf(const char *format, ...);
s32 console_write(char *string);
s32 console_write_char(char);
char console_read_char(void);

#endif /* CONSOLE_H */
