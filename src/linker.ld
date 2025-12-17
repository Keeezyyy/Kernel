
ENTRY(kernel_main)


PHDRS
{
  text   PT_LOAD FLAGS(5); 
  rodata PT_LOAD FLAGS(4); 
  data   PT_LOAD FLAGS(6); 
}

SECTIONS
{
  . = 0xffffffff80000000;
  kernel_start = .;


  .rodata : ALIGN(4K)
  {
    KEEP(*(.limine_requests_start))
    KEEP(*(.limine_requests))
    KEEP(*(.limine_requests_end))
    *(.rodata .rodata.*)
  } :rodata

  .text : ALIGN(4K)
  {
    *(.text .text.*)
  } :text


  .init_array : ALIGN(4K)
  {
    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .;
  } :data


  .data : ALIGN(4K)
  {
    *(.data .data.*)
  } :data


  .bss : ALIGN(4K)
  {
    *(COMMON)
    *(.bss .bss.*)
  } :data

  kernel_end = .;

  {
    *(.eh_frame*)
    *(.rela.*)
    *(.got*)
    *(.plt*)
    *(.iplt*)
    *(.igot*)
    *(.note*)
    *(.comment*)
  }
}

