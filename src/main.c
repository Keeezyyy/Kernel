#include "./limine/limine.h"

#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

#define LIMINE_MEMMAP_REQUEST_ID {LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62}

void putpixel(limine_framebuffer *fb, int x, int y,
              uint8_t r, uint8_t g, uint8_t b)
{

    uint8_t *addr = fb->address;
    uint64_t pitch = fb->pitch;

    uint32_t *pixel = (uint32_t *)(addr + y * pitch + x * 4);
    *pixel = (r << fb->red_mask_shift) | (g << fb->green_mask_shift) | (b << fb->blue_mask_shift);
}

void kernel_main()
{
    request();

    limine_framebuffer *fb = getFB();

    uint8_t *fb_addr = (uint8_t *)fb->address;
    uint64_t pitch = fb->pitch;
    uint64_t bpp = fb->bpp; 
    uint64_t width = fb->width;
    uint64_t height = fb->height;

    putpixel(fb, 100, 100, 255, 0, 0);


    for (;;)
    {
    }
}
