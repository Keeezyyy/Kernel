#include "./vmm_new.h"


void vmm_init(){
  
}

//cr3 logic 
//------------------------------------------------------------------------------------------
static inline uint64_t get_cr3()
{
  uint64_t cr3 = 0;
  asm("mov %%cr3, %0\n" : "=r"(cr3));
  return cr3;
}
static inline void reload_cr3(){
     asm volatile (
        "mov %%cr3, %%rax\n"
        "mov %%rax, %%cr3\n"
        :
        :
        : "rax", "memory"
    );
}
static inline void* get_va_from_physical_address(uint64_t physical_address){
  return (void*)DIRECT_MAPPING_OFFSET + physical_address;
}
//------------------------------------------------------------------------------------------



