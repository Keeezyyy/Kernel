#include "./x86-64/stdio/stdio.h"
#include "./x86-64/stdio/font/font.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"
#include "./x86-64/memory/memory.h"
#include "./x86-64/memory/pmm.h"
#include "./x86-64/memory/vmm.h"
#include "./x86-64/asm_connection/asm_connect.h"
#include "./x86-64/pic/8259_pic.h"
#include "./limine/limine.h"
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

  init_pml4();
  uint64_t new_fb_address = init_framebuffer_mapping();
  finilize_new_pml4();


  //new paging ->---------------------------------------------------------------------------------------------
  set_new_address(new_fb_address);
  clear_screen();
  resPos();


  printf("HELLLLLLLO WORLD\n");

  vmm_init();

  init_pic_8259();

  // Keyboard (IRQ1) AN
  pic_8259_clear_mask(1);  //maks out the timer interrupt

  enable_irq();
  for (;;)
  {
  }
}
