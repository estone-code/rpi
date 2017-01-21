.globl _start
_start:
	mov sp,#0x00010000
	bl sysmain
loop:	b loop
