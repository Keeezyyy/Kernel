#include "./stdio.h"

unsigned int posX = 5;
unsigned int posY = 5;

uint32_t screen_width = 0;
uint32_t screen_height = 0;

bool cursor_active;

struct limine_framebuffer *fb_stdio;


void clear_screen(){

  //for(int i = 0; i< fb_stdio->width;i++){
  for(int i = 0; i< screen_width;i++){
    //for(int k = 0; k< fb_stdio->height;k++){
    for(int k = 0; k<screen_height ;k++){
      putpixel(i, k, 0, 0, 0);
    }
  }
  resPos();
}
void initSDTIO(void *framebuffer)
{

  fb_stdio = (struct limine_framebuffer *)framebuffer;
  init_font((uint64_t)((struct limine_framebuffer *)framebuffer)->address,fb_stdio->pitch,  fb_stdio->red_mask_shift, fb_stdio->green_mask_shift, fb_stdio->blue_mask_shift);

  screen_height = fb_stdio->height;
  screen_width = fb_stdio->width;
}
void resPos(){
  posX = 5; 
  posY = 5;
}
void incPos()
{
  posX += VERTICAL_LETTER_SPACING + LETTER_WIDTH;

  // TODO detect max chars per row reacged
  // TODO implement scrolling 
  // posY += HORIZONTAL_LETTER_SPACING + LETTER_HEIGHT;
}

void putc(char c)
{
  if (c == '\n')
  {
    posY += HORIZONTAL_LETTER_SPACING + LETTER_HEIGHT;
    posX = 5;
    return;
  }

  clearChar( 200, 200, (posX), (posY));
  drawPixel(c, 200, 200, (posX), (posY));
  incPos();
}

void printHexDump(void *adr, uint16_t lines)
{

  uint8_t *byteAdr = adr;
  for (int i = 0; i < lines; i++)
  {
    printf("0x%x\0", (uint32_t)byteAdr);

    for (int k = 0; k < 8; k++)
    {
      printf(" ");
      putByte(*(byteAdr));
      printf(" ");
      // printf("  %b  ", *(uint8_t*)(adr));
      byteAdr++;
    }

    printf("\n\0");
    byteAdr++;
  }
}

void putByte(const unsigned char val)
{
  unsigned char high = (val >> 4) & 0x0F;
  unsigned char low = val & 0x0F;

  if (high < 0x0A)
    putc(high + '0');
  else
    putc(high - 0x0A + 'A');

  if (low < 0x0A)
    putc(low + '0');
  else
    putc(low - 0x0A + 'A');
}

void putS(const char *str)
{
  while (*str != '\0')
  {
    putc(*str++);
  }
}

//TODO: fix calling conventions error 
void printf(const char *str, ...)
{
  va_list ap;
  va_start(ap, str);

  while (*str)
  {
    if (*str == '%')
    {
      str++;
      switch (*str)
      {
        case 'x': {
          uint32_t val = va_arg(ap, uint32_t);
          for (int i = 3; i >= 0; i--)
            putByte((val >> (i * 8)) & 0xFF);
          break;
        }

        case 'p': {
          uint64_t val = va_arg(ap, uint64_t);
          for (int i = 7; i >= 0; i--)
            putByte((val >> (i * 8)) & 0xFF);
          break;
        }

        case 's': {
          char *s = va_arg(ap, char *);
          putS(s);
          break;
        }

        case 'c': {
          int ci = va_arg(ap, int);   
          putc((char)ci);
          break;
        }

        case 'b': {
          int bi = va_arg(ap, int);   
          putByte((uint8_t)bi);
          break;
        }

        default:
          putc('%');
          putc(*str);
          break;
      }
    }
    else
  {
      putc(*str);
    }
    str++;
  }

  va_end(ap);
}


void toggle_cursor(){

  if(cursor_active){
    cursor_active = false;
    drawPixel('_', 200, 200, (posX), (posY));
  }else{
    cursor_active = true;
    clearChar( 200, 200, (posX), (posY));
  }
}
