#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

s32 console_init(void);
s32 console_write(char *string);
s32 console_write_char(char);
s32 console_write_hex(unsigned char c);
char console_read_char(void);

#endif /* CONSOLE_H */
