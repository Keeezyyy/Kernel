#pragma once
#include <stdint.h>
#include "./font/font.h"

#include <stdarg.h>

#define ALIGN_CEIL(x, align)  (((x) + (align) - 1) / (align)) * (align)

#define LETTER_HEIGHT 13
#define LETTER_WIDTH 8
#define VERTICAL_LETTER_SPACING 2
#define HORIZONTAL_LETTER_SPACING 5

extern struct limine_framebuffer *fb_stdio;   

void initSDTIO(void *fb);

void putc(char c);
void incPos();
void putS(const char *str);


void putByte(unsigned char val);
void printf(const char* str, ...);
void printHexDump(void* adr, uint16_t lines);

void *memset(void *d, int c, uint64_t len);
