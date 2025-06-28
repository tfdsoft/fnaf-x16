/* Commander X16/ZSMkit PRG file linker script.
 *
 * BASIC memory regions are freely overwritten, but any KERNAL regions are left alone.
 *
 * Produces a PRG file with a SYS command to start the program.
 */


PROVIDE(__hiram = 0xa000);
PROVIDE(__hiram_size = 0x2000);


__basic_zp_start = 0x0002;
__basic_zp_end = 0x0080;


MEMORY {
    ram (rw) : ORIGIN = 0x0801, LENGTH = 0x7EFF
    zsm (rw) : ORIGIN = 0x8700, LENGTH = 6122
    hiram (rw) : ORIGIN = __hiram, LENGTH = __hiram_size
}

INCLUDE commodore.ld

SECTIONS {
    .zsm_section : { KEEP(*(.zsm_section)) } > zsm
    .hiram : { *(.hiram_section .hiram.*) } > hiram
}

/* Set initial soft stack address (It grows down) */
__stack = 0x86ff;

OUTPUT_FORMAT {
    /* Tells the LOAD command where to place the file's contents. */
    SHORT(0x0801)
    FULL(ram)
    FULL(zsm)
}