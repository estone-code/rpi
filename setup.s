// If the ld-script, pi bootloader, and config.txt
// are all configured correctly, this should be
// where program-counter is initilized to after
// kernel.img is loaded into ram.
// We enter into 'c' code by first initializing the
// stack-pointer, then branching to our 'sysmain' function.
// A safety spin-loop follows just in case the sysmain
// function ever returns (it shouldn't).
.global _start

.text

_start:
	MOV	SP, #0x100000	// 1 MiB
	BL	sysmain

spin:
	B	spin
