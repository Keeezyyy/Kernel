
ENTRY(kernel_main)

/* -------------------------------------------------- */
/* Program headers (MUST be before SECTIONS)          */
/* -------------------------------------------------- */
PHDRS
{
  text   PT_LOAD FLAGS(5); /* R + X */
  rodata PT_LOAD FLAGS(4); /* R */
  data   PT_LOAD FLAGS(6); /* R + W */
}

/* -------------------------------------------------- */
/* Sections                                           */
/* -------------------------------------------------- */
SECTIONS
{
  . = 0xffffffff80000000;
  kernel_start = .;

  /* -------------------------------------------------- */
  /* Read-only data (including Limine requests)         */
  /* -------------------------------------------------- */
  .rodata : ALIGN(4K)
  {
    KEEP(*(.limine_requests_start))
    KEEP(*(.limine_requests))
    KEEP(*(.limine_requests_end))
    *(.rodata .rodata.*)
  } :rodata

  /* -------------------------------------------------- */
  /* Code                                               */
  /* -------------------------------------------------- */
  .text : ALIGN(4K)
  {
    *(.text .text.*)
  } :text

  /* -------------------------------------------------- */
  /* Constructors                                       */
  /* -------------------------------------------------- */
  .init_array : ALIGN(4K)
  {
    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .;
  } :data

  /* -------------------------------------------------- */
  /* Data                                               */
  /* -------------------------------------------------- */
  .data : ALIGN(4K)
  {
    *(.data .data.*)
  } :data

  /* -------------------------------------------------- */
  /* BSS                                                */
  /* -------------------------------------------------- */
  .bss : ALIGN(4K)
  {
    *(COMMON)
    *(.bss .bss.*)
  } :data

  kernel_end = .;

  /* -------------------------------------------------- */
  /* Remove unwanted ELF / runtime sections             */
  /* -------------------------------------------------- */
  /DISCARD/ :
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

