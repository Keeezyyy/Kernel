#include "./vmm.h"
//TODO: make this dynamic by searching through memmap 
#define PAGE_TABLE_OFFSET 0x10000

static uint64_t end_line = 0;

static uint64_t starting_phyical;
static uint64_t starting_virtual;

void vmm_init(uint64_t phy_start, void *PML4){
  starting_phyical = phy_start;
  starting_virtual = (uint64_t)PML4;
}



uint64_t get_new_page_table_address(){
  return PAGE_TABLE_OFFSET;
}


void load_initial_page_tables(void* start, uint32_t length, void* buffer){
  end_line = PAGE_TABLE_OFFSET + length;

  printf("start : 0x%p, length : 0x%p , buffer_adr : 0x%p\n", (uint64_t)start, length, (uint64_t)buffer);

  memcpy(buffer, start, length);

  printf("copied paage tables \n");

  uint64_t *entries = (uint64_t*)start;
  int found = 0;
}


void *vmm_alloc_pages(uint32_t size_in_pages, void* starting_adr){
  
}

uint64_t get_cr3(){
  uint64_t cr3 = 0;
  asm ("mov %%cr3, %0\n" :"=r"(cr3));
  return cr3;
}
