MEMORY
{
    /*
     * max LENGTH is a function of the cpu/gpu split that
     * should be set in config.txt
     * For now giving 1 MiB (256 pages)
     */
     ram : ORIGIN = 0x0, LENGTH = 0x100000
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .bss : { *(.bss*) } > ram
}
