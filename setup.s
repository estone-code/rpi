// If the ld-script, pi bootloader, and config.txt
// are all configured correctly, this should be
// where program-counter is initilized to after
// kernel.img is loaded into ram.
//
// For now, set 3 of the 4 cores spinning in a loop. Later
// improve this to be wait-for-interrupt etc.
//
// For the remaining main core, we enter into 'c' code by first initializing
// the stack-pointer, then branching to our 'sysmain' function.
// A safety spin-loop follows just in case the sysmain
// function ever returns (it shouldn't).
//
.global _start

.text

_start:
	// TODO need to get all the cores spinning except our main core
	MOV	SP, #0x100000	// 1 MiB
	BL	sysmain

spin:
	B	spin
