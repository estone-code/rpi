#include "console.h"
#include "string.h"
#include "system.h"

static char *strtok_ptr;

int strcmp(const char *s1, const char *s2)
{
	int diff;
	while (*s1) {
		if (!*s2) {
			return 1;
		}
		diff = *s1 - *s2;
		if (diff) {
			return diff;
		}
		s1++;
		s2++;
	}
	if (*s2) {
		return -1;
	}
	return 0;
}

size_t strlen(const char *str)
{
	size_t i;
	for (i=0; *str; str++) {
		i++;
	}
	return i;
}

char *strpbrk(const char *s, const char *accept)
{
	const char *c;
	for (; *s; s++) {
		for (c = accept; *c; c++) {
			if (*s == *c) {
				return (char *)s;
			}
		}
	}
	return 0;
}

size_t strspn(const char *s, const char *accept)
{
	const char *c;
	size_t i;
	u8 found;

	i = 0;
	for (; *s; s++) {
		found = 0;
		for (c = accept; *c; c++) {
			if (*s == *c) {
				found = 1;
				break;
			}
		}
		if (found) {
			i++;
		} else {
			break;
		}
	}
	return i;
}

char *strtok(char *str, const char *delim)
{
	size_t len;
	char *retval;

	if (!str) {
		str = strtok_ptr;
	}

	/* find the start of the next substring that has no chars in delim */
	len = strspn(str, delim);
	str += len;
	if (!*str) {
		strtok_ptr = str;
		return 0;
	}

	/* remember where we are, and find the next one */
	retval = str;
	str = strpbrk(retval, delim);
	if (str) {
		*str = '\0';
		strtok_ptr = str + 1;
	} else {
		len = strlen(retval);
		strtok_ptr = retval + len;
	}
	return retval;
}
