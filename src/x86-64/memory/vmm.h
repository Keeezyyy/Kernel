#pragma once 
#include "../stdio/stdio.h"
#include "./mem_utils.h"
#include <stdint.h>

#define PHYSICAL_ADDRESS_OFFSET 0x10000
void vmm_init(uint64_t starting_phyical, void *PML4);

void *vmm_alloc_pages(uint32_t size_in_pages, void* starting_adr);

void vmm_free_pages(void* ptr);

void load_initial_page_tables(void* start, uint32_t length, void* buffer);


uint64_t get_new_page_table_offset();
uint64_t get_cr3();
// pointer caching similular to cpu cache with queue structure 
//
//
//
//
// 
