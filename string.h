#ifndef STRING_H
#define STRING_H

#include "stddef.h"

size_t strlen(const char *str);
char *strpbrk(const char *s, const char *accept);
size_t strspn(const char *s, const char *accept);
char *strtok(char *str, const char *delim);

#endif /* STRING_H */
