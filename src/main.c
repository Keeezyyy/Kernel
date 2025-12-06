#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

#define LIMINE_MEMMAP_REQUEST_ID {LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62}



void kernel_main()
{
    request();

    limine_framebuffer *fb = getFB();

    uint8_t *fb_addr = (uint8_t *)fb->address;
    uint64_t pitch = fb->pitch;
    uint64_t bpp = fb->bpp; 
    uint64_t width = fb->width;
    uint64_t height = fb->height;





    char c = '1';
    for(int i = 0;i<25; i++){
        putc(c, fb);
        c++;
    }

    for (;;)
    {
    }
}
