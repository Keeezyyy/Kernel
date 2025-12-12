#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"
#include "./x86-64/memory/memory.h"
#include "./x86-64/asm_connection/asm_connect.h"
#include "./kernel/kernel.h"

#define STACK_SIZE 16384

static uint8_t stack[STACK_SIZE] __attribute__((aligned(4096)));

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void kernel_main()
{
  setStack(&stack[STACK_SIZE - 1]);

  request_return *request_values = request();

  initSDTIO(get_framebuffer());

  init_gdt();

  init_idt();

  //printf("%p\n", (uint64_t)get_hhdm()->offset);
  //printf("%p\n", (uint64_t)get_memmap);
  //printf("%p\n", (uint64_t)get_executable_address);
  //printf("%p\n", (uint64_t)get_framebuffer);
  //printf("%p\n", (uint64_t)get_executable_file);

  init_pml4();

  malloc_physical_address(convert_virtual_to_physical((uint64_t)get_framebuffer()), 1);

  for (;;)
  {
  }
}
