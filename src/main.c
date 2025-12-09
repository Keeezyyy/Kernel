#include "./limine/limine.h"
#include "./x86-64/stdio/stdio.h"
#include "./x86-64/gdt/gdt.h"
#include "./x86-64/idt/idt.h"
#include "./x86-64/memory/memory.h"
#include "./x86-64/asm_connection/asm_connect.h"
#include "./kernel/kernel.h"

#define STACK_SIZE 16384

static uint8_t PML4[STACK_SIZE] __attribute__((aligned(4096)));

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void kernel_main()
{
  setStack(&PML4[STACK_SIZE - 1]);

  request();


  initSDTIO(getFB());

  init_gdt();

  init_idt();


  init_pml4(getMEMMAP(), getHHDM(), getEXEC_ADDRESS(), getEXEC_FILE());

  for (;;)
  {
  }  
}
