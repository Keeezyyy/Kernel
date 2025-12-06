#pragma once
#include <stdint.h>
#include "./font/font.h"

#define LETTER_HEIGHT 13
#define LETTER_WIDTH 8
#define VERTICAL_LETTER_SPACING 3
#define HORIZONTAL_LETTER_SPACING 5


void putc(char c,limine_framebuffer *fb);
void incPos();
