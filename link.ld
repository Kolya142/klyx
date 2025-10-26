ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS {
    . = 0x7E00;

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
