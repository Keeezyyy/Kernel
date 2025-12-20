#pragma once 
#include "../stdio/stdio.h"
#include "../../kernel/kernel.h"
#include "./mem_utils.h"
#include <stdint.h>

#define bool uint8_t
#define true 1
#define false 0
#define NULL (void*)0x0
#define PAGE_TABLE_OFFSET 0xffffB00000000000ULL

typedef struct parsed_virtual_address parsed_virtual_address;
typedef struct pte_params pte_params;

struct vm_area{
  uint64_t start_address;
  uint64_t end_address;
};


enum area_type {
  kernel_heap,
};

parsed_virtual_address find_virtual_memory_slot(struct vm_area *area, uint32_t page_length);
inline struct vm_area get_area(enum area_type type);
inline void* convert_paging_phys_to_vir(uint64_t phy);
uint64_t get_present_slot_in_table(uint64_t* table, uint16_t starting_index, uint16_t ending_index);
uint64_t get_empty_slot_in_table(uint64_t* table, uint16_t starting_index, uint16_t ending_index, uint32_t length);
uint32_t find_contiguous_empty_slots(uint64_t *table, uint64_t **out,void *prev, uint32_t prev_length,  uint32_t length);
const parsed_virtual_address get_small_page_slot(uint64_t* pml4,uint16_t starting_index,uint16_t ending_index,uint32_t length);

void decode_page_table_params(uint64_t entry, page_table_params *p);
void* vmm_alloc(enum area_type type, uint32_t size);
void  vmm_free(struct vm_area* area, void* addr);

uint64_t get_new_page_table_offset();
uint64_t get_cr3();
// pointer caching similular to cpu cache with queue structure 
//
//
//
//
// 
