#pragma once
#include <stdint.h>
#include "../../../limine/limine.h"


#define bool uint8_t
#define true 1
#define false 0

#define LETTER_HEIGHT 13
#define PIXEL_SIZE 2




void drawPixel(char c, uint32_t max_x, uint32_t max_y, uint32_t start_x, uint32_t start_y,struct limine_framebuffer *fb);