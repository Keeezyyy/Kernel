#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/gdt/gdt.h"


void kernel_main()
{
    request();

    limine_framebuffer *fb = getFB();

    initSDTIO(fb);
    uint8_t *fb_addr = (uint8_t *)fb->address;
    uint64_t pitch = fb->pitch;
    uint64_t bpp = fb->bpp; 
    uint64_t width = fb->width;
    uint64_t height = fb->height;



    printf("this is the start of the kernel\n");


    printf("height %x\n", height);

    init_gdt();

  
    for (;;)
    {
    }
}
