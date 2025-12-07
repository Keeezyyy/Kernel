#include "./limine.h"

static struct limine_framebuffer *fb;

struct limine_hhdm_response *hhdm;

__attribute__((used, section(".limine_requests_start"))) static const uint64_t limine_requests_start_marker[4] = {0xf6b8f4b39de7d1ae, 0xfab91a6940fcb9cf, 0x785c6ed015d3e316, 0x181e920a7852b9d9};

__attribute__((used, section(".limine_requests"))) volatile struct limine_base_revision base_revision_request = {
  0xf9562b2d5c95a6c8,
  0x6a7b384944536bdc,
  4};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_hhdm_request hhdm_request = {
  .id = LIMINE_HHDM_REQUEST_ID,
  .revision = 0,
};
__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
  .revision = 0,
};


__attribute__((used, section(".limine_requests_end"))) static const uint64_t limine_requests_end_marker[2] = {0xadc0e0531bb10d03, 0x9572709f31764c62};

void request()
{
  struct limine_framebuffer_response *fb_resp =
    (struct limine_framebuffer_response *)framebuffer_request.response;

  struct limine_framebuffer *tmp = fb_resp->framebuffers[0];

  fb = tmp;

  // // hhdm
  // hhdm = hhdm_request.response;
  struct limine_hhdm_response *tmp_hhdm= (struct limine_hhdm_response*)hhdm_request.response;


  hhdm = tmp_hhdm;

  volatile uint64_t val = hhdm->offset;


  if (val == NULL) {
    //somthing wrong with hhdm
    for(;;){}
  }


}

struct limine_framebuffer *getFB()
{
  return fb;
}

struct limine_hhdm_response *getHHDM()
{
  return hhdm;
}
