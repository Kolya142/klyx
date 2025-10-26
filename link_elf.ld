ENTRY(_start)

SECTIONS {
    . = 0x7E00;

    _start = .;

    .text : {
        KEEP(*(.init))
        *(.text*)
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    _end = .;
}
