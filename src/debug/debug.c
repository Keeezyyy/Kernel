#include "debug.h"

#include "../../include/printf/printf.h"
#include "../../src/drivers/seriel-port/seriel-port.h"
#include <stdarg.h>
#include <stdint.h>

static uint8_t seriel_port = 0;

static void _print_char_uart(char c) { write_serial(c, seriel_port); }

void init_debug_output(uint8_t sp) {
  seriel_port = sp;

  init_printf(&_print_char_uart);
}

void debug_printf(const char *format, ...) {
  va_list va;
  va_start(va, format);
  vprintf_(format, va);
  va_end(va);
}

void debug_assert(int e) {
  if (!e) {
    panic("assert failed!");
  }
}

void panic(char *r) {
  debug_printf("[PANIC] : %s\n", r);
  for (;;) {
    __asm__ volatile("hlt");
  }
}
