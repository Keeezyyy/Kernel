#include "./stdio.h"

unsigned int posX = 5;
unsigned int posY = 5;

void incPos()
{
    posX += VERTICAL_LETTER_SPACING + LETTER_WIDTH;

    // TODO detect max chars per row reacged

    // posY += HORIZONTAL_LETTER_SPACING + LETTER_HEIGHT;
}

void putc(char c, limine_framebuffer *fb)
{
    if (c == '\n')
    {
        posY += HORIZONTAL_LETTER_SPACING + LETTER_HEIGHT;
        posX = 5;
        return;
    }

    drawPixel(c, 200, 200, (posX), (posY), fb);
    incPos();
}
