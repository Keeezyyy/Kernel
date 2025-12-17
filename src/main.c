#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/stdio/font/font.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"
#include "./x86-64/memory/memory.h"
#include "./x86-64/memory/pmm.h"
#include "./x86-64/asm_connection/asm_connect.h"
#include "./kernel/kernel.h"
#include "./utils/utils.h"

#define STACK_SIZE 16384

static uint8_t stack[STACK_SIZE] __attribute__((aligned(4096)));

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];



void kernel_main()
{
  setStack(&stack[STACK_SIZE - 1]);

  request_return *request_values = request();

  struct limine_framebuffer *fb = get_framebuffer();


  initSDTIO(fb);

  init_gdt();

  init_idt();

  getByteSize();

  getPhyAdr();

  uint64_t new_fb_address = init_pml4();
  finilize_new_pml4();


  //new paging ->---------------------------------------------------------------------------------------------
  set_new_address(new_fb_address);
  clear_screen();
  resPos();

  for(int i = 0; i < 0x9C; i++)
    pmm_alloc_frame();

  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  printf("next availible page : 0x%p\n", pmm_alloc_frame());
  
  for (;;)
  {
  }
}
