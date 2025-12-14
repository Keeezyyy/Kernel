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

uint64_t convert_virtual_to_physical(uint64_t virtual){


  if(physical_entry == 0 && virtual_entry == 0)
    printf("kernel conversion not initilized !!!");
  return physical_entry + (virtual- virtual_entry);
}

void kernel_panic(char* str){
  printf("%s\n", str);
  kernel_halt();

}

