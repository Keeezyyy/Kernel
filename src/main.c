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

void _start(void);

void _start(void) {
  _initilizer();

  debug_printf("hello world!\n");

  for (;;) {
    __asm__ volatile("hlt");
  }
}
