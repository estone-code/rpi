#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

/*
 * implemented in assembly
 */
void noop();

/*
 * write memory barrier
 */
void wmb();

/*
 * read memory barrier
 */
void rmb();

void delay(u32 cycles);

#endif
