#pragma once

#include <stdarg.h>
#include <stdint.h>

void init_debug_output(uint8_t sp);

void panic(char *r);

void debug_assert(int);

void debug_printf(const char *format, ...);
