#include "./limine.h"

 request_return request_return_values;

static struct limine_memmap_response *memmap_res = NULL;
static struct limine_hhdm_response *hhdm_res = NULL;
static struct limine_executable_address_response *exe_addr_res = NULL;
static struct limine_framebuffer *fb_res = NULL;
static struct limine_executable_file_response *exe_file_res = NULL;


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
__attribute__((used, section(".limine_requests"))) static volatile struct limine_memmap_request memmmap_request = {
  .id = LIMINE_MEMMAP_REQUEST_ID,
  .revision = 0,
};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_executable_address_request exec_address_request = {
  .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
  .revision = 0,
};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_executable_file_request exec_file_request = {
  .id = LIMINE_EXECUTABLE_FILE_REQUEST_ID,
  .revision = 0,
};
__attribute__((used, section(".limine_requests_end"))) static const uint64_t limine_requests_end_marker[2] = {0xadc0e0531bb10d03, 0x9572709f31764c62};

request_return *request(void)
{

  fb_res = framebuffer_request.response->framebuffers[0];
  request_return_values.fb = fb_res;

  if(fb_res == 0x0){
    while (1) {
    }
  }

  memmap_res = memmmap_request.response;
  request_return_values.memmap = memmap_res;

  if( memmap_res== 0x0){
    while (1) {
    }
  }


  exe_addr_res = exec_address_request.response; 
  request_return_values.exe_address = exe_addr_res;

  if( exe_addr_res== 0x0){
    while (1) {
    }
  }


  exe_file_res = exec_file_request.response;
  request_return_values.exe_fil = exe_file_res;

  if( exe_file_res== 0x0){
    while (1) {
    }
  }


  hhdm_res = hhdm_request.response;
  request_return_values.hhdm =hhdm_res;

  if( hhdm_res== 0x0){
    while (1) {
    }
  }


  return &request_return_values;
}
struct limine_hhdm_response *getHHDM(){
  return hhdm_request.response;
}

struct limine_memmap_response *get_memmap(void)
{
    return memmap_res;
}

struct limine_hhdm_response *get_hhdm(void)
{
    return hhdm_res;
}

struct limine_executable_address_response *get_executable_address(void)
{
    return exe_addr_res;
}

struct limine_framebuffer *get_framebuffer(void)
{
    return fb_res;
}

struct limine_executable_file_response *get_executable_file(void)
{
    return exe_file_res;
}


uint64_t getByteSize(struct limine_framebuffer *fb)
{
  return fb->pitch * fb->height;
}

uint64_t getPhyAdr(struct limine_framebuffer *fb, struct limine_hhdm_response *hhdm)
{
  return (uint64_t)(fb->address - hhdm->offset);
}
