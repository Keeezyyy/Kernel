#include "./kernel.h"

static uint64_t physical_entry;
static uint64_t virtual_entry;

struct kernel_region get_kernel_region(void) {
}

void init_conversion(uint64_t phy, uint64_t vir){
  printf("phy : 0x%p\n", phy);
  printf("vir : 0x%p\n", vir);
  physical_entry = phy;
  virtual_entry = vir;
}

uint64_t convert_virtual_to_physical(uint64_t virtual_adr){


  if(physical_entry == 0 && virtual_entry == 0)
    printf("kernel conversion not initilized !!!");
  return physical_entry + (virtual_adr- virtual_entry);
}

uint64_t convert_physical_to_virtual(uint64_t physical_adr){
  return virtual_entry + (physical_adr - physical_entry);
}

void cpuid(uint32_t leaf,uint32_t* eax,uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile("cpuid"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(leaf), "c"(0));
}
void kernel_panic(char* str){
  printf("[PANIC] : ");
  printf("%s\n", str);
  kernel_halt();

}
void enable_irq(void){
  asm volatile ("sti");
}

