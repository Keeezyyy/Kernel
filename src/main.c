#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"
#include "./x86-64/memory/memory.h"


void kernel_main()
{
  request();



  struct limine_framebuffer *fb = getFB();

  initSDTIO(fb);
  uint8_t *fb_addr = (uint8_t *)fb->address;
  uint64_t pitch = fb->pitch;
  uint64_t bpp = fb->bpp; 
  uint64_t width = fb->width;
  uint64_t height = fb->height;


  init_gdt();

  init_idt();

  init_pml4(getMEMMAP(), getHHDM());

  for (;;)
  {
  }  
}
