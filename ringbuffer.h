#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "types.h"

struct ringbuffer {
	u8 * const buf;
	const int size;
	int head;
	int tail;
};

#define RINGBUFFER(name, size)		\
	u8 name##_space[(size)];	\
	struct ringbuffer (name) = {	\
		name##_space,		\
		(size),			\
		0,			\
		0			\
	};

s32 enqueue(struct ringbuffer *buf, u8 data);

s32 dequeue(struct ringbuffer *buf, u8 *data);

#endif /* RINGBUFFER_H */
