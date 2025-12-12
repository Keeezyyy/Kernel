
ENTRY(kernel_main)

SECTIONS {
  . = 0xffffffff80000000;
  kernel_start = .;

  .limine_requests : {
      KEEP(*(.limine_requests_start))
      KEEP(*(.limine_requests))
      KEEP(*(.limine_requests_end))
  }

  .text : {
      *(.text*)
  }

  .rodata : {
      *(.rodata*)
  }

  .data : {
      *(.data*)
  }

  .bss : {
      *(.bss*)
      *(COMMON)
  }

  kernel_end = .;
}



