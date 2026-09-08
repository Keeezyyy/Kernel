#include <stdint.h>

#include "../include/bootboot.h"
#include "./debug/debug.h"
#include "./drivers/seriel-port/seriel-port.h"
#include "./lib/string.h"
#include "arch/x86-64/gdt/gdt.h"
#include "arch/x86-64/idt/idt.h"

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
  output -> int 0xE
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
