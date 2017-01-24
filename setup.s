// If the ld-script, pi bootloader, and config.txt
// are all configured correctly, this should be
// where program-counter is initilized to after
// kernel.img is loaded into ram.
//
// After the bootloader releases the ARM cores' reset,
// all 4 of them will start executing the same code
// from the same initial PC. To bring order to this
// potential chaos, the first code we'll execute is
// for each core to examine its 'id', and if it's not
// '0' (there can be only one...), go into an infinite
// loop.  (Later might improve on this to instead be
// wait-for-interrupt). Then for the rest of the code
// we can think there's only just the single core.
//
.text

.global _start
_start:
	// If our core-id (found in the bottom nibble of the mpidr_el1 system
	// register, see the A53's datasheet from arm) is non-zero, goto spin
	mrs	x0, mpidr_el1
	and	x0, x0, #0xF
	cbnz	x0, spin
	// only the "first" core should make it here. Initialize its
	// stack-pointer and call the sysmain C function
	mov	sp, #0x100000	// 1 MiB
	bl	sysmain
	// if sysmain ever accidentally returns, fall through to spin
spin:
	b	spin
