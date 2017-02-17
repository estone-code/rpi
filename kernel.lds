MEMORY
{
    /*
     * max LENGTH is a function of the cpu/gpu split that
     * should be set in config.txt
     * For now giving 1 MiB (256 pages)
     */
     ram : ORIGIN = 0x0, LENGTH = 0x100000
}

/*
 * https://sourceware.org/binutils/docs/ld/Source-Code-Reference.html#Source-Code-Reference
 */
SECTIONS
{
    .text (0x0) : { *(.text*) } > ram
    .rodata : { *(.rodata*) } > ram
    .data :  { *(.data*) } > ram
    .bss : ALIGN(8) {
        _START_OF_BSS = .;
        *(.bss*)
        . = ALIGN(8);
        _END_OF_BSS = .;
    } > ram

}
