
ENTRY(kernel_main)

SECTIONS {
  . = 0xffffffff80000000;

  .text : { *(.text*) }

  .rodata : {
      *(.rodata*)

      KEEP(*(.limine_requests_start))
      KEEP(*(.limine_requests))
      KEEP(*(.limine_requests_end))
  }

  .data : { *(.data*) }

  .bss : { *(.bss*) *(COMMON) }
}

