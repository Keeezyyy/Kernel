#pragma once
#include <stdint.h>
#include "../../limine/types/limine_types.h"

#define ALIGN_UP_0x1000(x)  (((x) + 0xFFF) & ~0xFFF)

struct mem_area{
  uint64_t base;
  uint64_t top;
};


void claim_page(uint64_t page);

void init_memmap(struct limine_memmap_response *memmap_res, struct limine_hhdm_response *hhdm_res);
