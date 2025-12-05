#include "./limine.h"

limine_memmap_entry *mmap;

limine_framebuffer *fb;

__attribute__((used, section(".limine_requests"))) static volatile struct
{
    uint64_t id[4];
    uint64_t revision;
    void *response;
} fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = 0};

__attribute__((used, section(".limine_requests"))) static volatile struct
{
    uint64_t id[4];
    uint64_t revision;
    void *response;
} mmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
    .response = 0};

void request()
{
    limine_framebuffer_response *fb_resp =
        (limine_framebuffer_response *)fb_request.response;

    if (fb_resp == NULL || fb_resp->framebuffer_count < 1)
    {
        fb = NULL; 
        return;
    }

    fb = fb_resp->framebuffers[0];
}

limine_memmap_entry  *get_mmap()
{
    return mmap;
}

limine_framebuffer *getFB()
{
    return fb;
}