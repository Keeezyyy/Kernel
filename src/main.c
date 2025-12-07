#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"


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

  struct limine_hhdm_response *hhdm= getHHDM();

  volatile uint64_t offset = hhdm->offset;



  printf("val : %p\n", offset);

  for (;;)
  {
  }  
}
