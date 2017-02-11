#include "stdarg.h"
#include "stdio.h"
#include "types.h"

static void scan_decimal(const char **str_ptr, s32 *val_ptr) {
	s32 val;
	s8 sign;
	const char *str;

	str = *str_ptr;

	val = 0;
	if (*str == 0x2D) { /* '-' */
		sign = -1;
		str++;
	} else {
		sign = 1;
	}

	for (; *str >= '0' && *str <= '9'; str++) {
		val *= 10;
		val += *str - '0'; /* ascii conversion trick */
	}
	*val_ptr = sign * val;
	*str_ptr = str;
}

static void scan_hex(const char **str_ptr, u64 *val_ptr) {
	u64 val;
	const char *str;

	str = *str_ptr;
	val = 0;

	for (; (*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f')
		     || (*str >= 'A' && *str <= 'F'); str++) {
		val <<= 4; /* *= 16 */

		if (*str >= '0' && *str <= '9') {
			/* ascii conversion trick */
			val += *str - '0';
		} else {
			switch (*str) {
			case 'a':
			case 'A':
				val += 10;
				break;
			case 'b':
			case 'B':
				val += 11;
				break;
			case 'c':
			case 'C':
				val += 12;
				break;
			case 'd':
			case 'D':
				val += 13;
				break;
			case 'e':
			case 'E':
				val += 14;
				break;
			case 'f':
			case 'F':
				val += 15;
				break;
			}
		}
	}
	*val_ptr = val;
	*str_ptr = str;	
}


/*
 * A very incomplete and buggy implementation of sscanf.
 */
int sscanf(const char *str, const char *format, ...)
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
			case 'd':
				scan_decimal(&str, va_arg(ap, s32 *));
				break;
			case 'x':
				scan_hex(&str, va_arg(ap, u64 *));
				break;
			}
		} else {
			if ('%' == *c) {
				in_escape = 1;
			} else {
				/* ignore this char */
			}
		}
	}
	va_end(ap);
	return 0;
}
