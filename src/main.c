#include <stdint.h>

#include "../include/bootboot.h"
#include "./debug/debug.h"
#include "./drivers/seriel-port/seriel-port.h"
#include "./lib/string.h"
#include "arch/x86-64/gdt/gdt.h"
#include "arch/x86-64/idt/idt.h"
#include "arch/x86-64/mmap/mmap.h"
#include "mem/pmm.h"

extern BOOTBOOT bootboot;
extern uint8_t environment[4096];
extern uint8_t fb;

static void _initilizer(void) {

  uint8_t port_num = 0;
  if (init_seriel_port(38400, port_num, false, false) == 1) {
    return;
  }

  init_debug_output(port_num);

  init_gdt();

  init_idt();

  init_mmap(&bootboot);

  init_pmm();
}

static void _test(void) {

  // idt test
  /*
  volatile int a = 5;
  volatile int b = 0;
  volatile int c = a / b;

  output -> int 0x0

  __asm__ volatile("ud2");
  output -> int 0x6

  *(volatile uint64_t *)0x0000700000000000ull = 1;
  output->int 0xE

  // pmm alloc test

  u64 val = 0;
  pmm_alloc(&val);
  debug_printf("[%d] : adr :0x%016llx \n", 0, val);

  pmm_alloc(&val);
  debug_printf("[%d] : adr :0x%016llx \n", 1, val);

  pmm_alloc(&val);
  debug_printf("[%d] : adr :0x%016llx \n", 2, val);

  debug_printf("ret : %d\n", pmm_free(val)); -> 0
  debug_printf("ret : %d\n", pmm_free(0x0000000000015000)); -> 0
  debug_printf("ret : %d\n", pmm_free(val)); -> 1 already freed

  */
}

void _start(void);

void _start(void) {
  _initilizer();

  _test();

  for (;;) {
    __asm__ volatile("hlt");
  }
}
