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

// The linker MUST be scripted to place this code at address 0.
.global _start
_start:
	// Initialize the exception vector table for exception-level 1.
	// Each core gets the same table set. BCM2836 and 7 route all
	// IRQs to just one core. Which one, is configurable through a
	// peripheral register.
	mov	x0, exception_vector_table
	msr	vbar_el1, x0
	// If our core-id (found in the bottom nibble of the mpidr_el1 system
	// register, see the A53's datasheet from arm) is non-zero, we're
	// not the boot-core, and goto init_aux_core
	mrs	x0, mpidr_el1
	and	x0, x0, #0xF
	cbnz	x0, init_aux_core
	// only the "first" (boot) core should make it here.
	// Initialize its stack-pointer and call the sysmain C function
	mov	sp, #0x100000	// 1 MiB
	bl	sysmain
	// if sysmain ever accidentally returns, fall through to spin.
	// later this should instead invoke an 'oops' procedure.
spin:
	b	spin

init_aux_core:
	// for now there is no SMP support, so we set the non-boot
	// cores to wait in low-power wfe state. If they ever get
	// woken up, just loop back into wfe state.
wait:
	wfe
	b	wait

// it's not strictly necessary to align this to 128 bytes, but it makes
// using assembler directives to create algined padding in the table easier.
// the .balign's can be a little redundant but it's defensive
align_vector_table:
	.balign 128
// do not mark this .global, gas marks its address as 0x0 then
exception_vector_table:
// Current EL with SP0
	// Syncrhonous
	.balign 128
	.zero 128
	// IRQ/vIRQ
	.balign 128
	.zero 128
	// FIQ/vFIQ
	.balign 128
	.zero 128
	// SError/vSError
	.balign 128
	.zero 128
// Current EL with SPx
	// Syncrhonous
	.balign 128
	.zero 128
	// IRQ/vIRQ
	.balign 128

	.zero 128 // TODO replace this with:
	// store registers
	// goto c handler
	// restore registers
	// return back to where we were

	// FIQ/vFIQ
	.balign 128
	.zero 128
	// SError/vSError
	.balign 128
	.zero 128
// Lower EL using AArch64
	// Syncrhonous
	.balign 128
	.zero 128
	// IRQ/vIRQ
	.balign 128
	.zero 128
	// FIQ/vFIQ
	.balign 128
	.zero 128
	// SError/vSError
	.balign 128
	.zero 128
// Lower EL using AArch128
	// Syncrhonous
	.balign 128
	.zero 128
	// IRQ/vIRQ
	.balign 128
	.zero 128
	// FIQ/vFIQ
	.balign 128
	.zero 128
	// SError/vSError
	.balign 128
	.zero 128

// has linker place the address of exception_vector_table into
// a variable named exception_table_addr.
.global exception_table_addr
align_table_addr:
	.balign 8
exception_table_addr:
	.word exception_vector_table
