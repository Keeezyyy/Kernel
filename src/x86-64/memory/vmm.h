#pragma once 
#include "../stdio/stdio.h"
#include "../../kernel/kernel.h"
#include "./mem_utils.h"
#include <stdint.h>

#define SWAP_PAGE_LENGTH 1

#define bool uint8_t
#define true 1
#define false 0
#define NULL (void*)0x0
#define PAGE_TABLE_OFFSET 0xffffB00000000000ULL

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#ifndef PTE_PRESENT
#define PTE_PRESENT (1ULL << 0)
#endif

#ifndef PTE_PS
#define PTE_PS      (1ULL << 7)   
#endif

typedef struct parsed_virtual_address parsed_virtual_address;
typedef struct pte_params pte_params;

struct vm_area{
  uint64_t start_address;
  uint64_t end_address;
};


enum area_type {
  kernel_heap,
};

parsed_virtual_address    find_virtual_memory_slot(struct vm_area *area, uint32_t page_length);
inline struct vm_area     get_area(enum area_type type);
inline void*              convert_paging_phys_to_vir(uint64_t phy);
inline uint64_t           convert_paging_vir_to_phys(void* vir);
uint64_t                  get_present_slot_in_table(uint64_t* table, uint16_t starting_index, uint16_t ending_index);
int get_empty_slot_in_table(uint64_t *table, uint16_t starting_index, uint16_t ending_index, uint32_t length);
uint32_t find_contiguous_empty_slots(uint64_t *table, uint64_t **out,void *prev, uint32_t prev_length,  uint32_t length);
const parsed_virtual_address get_small_page_slot(uint64_t* pml4,uint16_t starting_index,uint16_t ending_index,uint32_t length);
void parse_indices_to_pointer(parsed_virtual_address indices, uint64_t **out_pml4, uint64_t **out_pdpt, uint64_t **out_pd, uint64_t **out_pt);
inline uint64_t construct_pte(pte_params params);
static inline uint64_t alloc_zero_table_page(void);
void init_vmm();
void fill_empty_memory_slot(parsed_virtual_address indices, uint32_t length);
inline int get_present_entry_index(uint64_t *table);
void init_swap_page();
void *load_physical_page_into_swap_page(uint64_t physical_page_address);

parsed_virtual_address alloc_new_4k_mapping_slot(uint64_t *pml4, uint16_t starting_index, uint16_t ending_index);
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
