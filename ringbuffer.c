#include "ringbuffer.h"

s32 enqueue(struct ringbuffer *buf, u8 data)
{
	int next;

	next = buf->head + 1;
	if (next >= buf->size) {
		next = 0;
	}
	if (next == buf->tail) {
		/* buffer is full */
		return -1;
	}
	buf->buf[buf->head] = data;
	buf->head = next;
	return 0;
}

s32 dequeue(struct ringbuffer *buf, u8 *data)
{
	int next;

	if (buf->head == buf->tail) {
		/* buffer is empty */
		return -1;
	}
	*data = buf->buf[buf->tail];
	buf->buf[buf->tail] = 0; /* zero out old data */
	next = buf->tail + 1;
	if (next > buf->size) {
		next = 0;
	}
	buf->tail = next;
	return 0;
}
