#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"
#include "../../limine/types/limine_types.h"

#define ALIGN_UP_0x1000(x)  (((x) + 0xFFF) & ~0xFFF)

struct mem_area{
  uint64_t base;
  uint64_t top;
};

typedef struct{
  uint8_t  present;
  uint8_t  rw;
  uint8_t  us;
  uint8_t  pwt;
  uint8_t  pcd;
  uint8_t  accessed;
  uint8_t  dirty;
  uint8_t  ps;
  uint8_t  global;
  uint8_t  avl;            
  uint64_t phys_addr;     
  uint8_t  nx;
} pte_params;
#define PTE_WRITE 1

typedef struct{
  uint16_t pml4_index;
  uint16_t pdpt_index;
  uint16_t pd_index;
  uint16_t pt_index;
  uint16_t offset;
} parsed_virtual_address;

void init_pml4(struct limine_memmap_response *, struct limine_hhdm_response *, struct limine_executable_address_response*, struct limine_executable_file_response *);
void read_memmap_into_buffer(struct limine_memmap_response *, struct limine_hhdm_response *);
void fill_initial_pages(struct limine_file* ,uint64_t , struct limine_executable_address_response *);

parsed_virtual_address parse_virtal_address(uint64_t address);

uint64_t get_pte_physical_addresse_from_virtual_address(uint64_t address, uint64_t base_virtual, uint64_t base_physical);
uint64_t make_pte(pte_params params);

extern void set_cr3(void*);
