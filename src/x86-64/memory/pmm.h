#pragma once
#include <stdint.h>
#include "../../limine/limine.h"
#include "../stdio/stdio.h"
#include "../../limine/types/limine_types.h"
#include "../../kernel/kernel.h"

#define ALIGN_UP_0x1000(x)  (((x) + 0xFFF) & ~0xFFF)

struct mem_area{
  uint64_t base;
  uint64_t top;
  uint32_t page_length;
};

void claim_page(uint64_t page);
void init_memmap();
uint32_t getBitmapIndex(uint64_t physical_address);
uint32_t convert_physical_to_bitmap_index(uint64_t physical_address);

#define phys_addr_t uint64_t

phys_addr_t pmm_alloc_frame();
void pmm_free_frame(phys_addr_t);
