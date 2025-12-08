#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"
#include "../../limine/types/limine_types.h"

struct mem_area{
  uint64_t base;
  uint64_t top;
};

void init_pml4(struct limine_memmap_response *, struct limine_hhdm_response *, struct limine_executable_address_response*, struct limine_executable_file_response *);
void read_memmap_into_buffer(struct limine_memmap_response *, struct limine_hhdm_response *);
void fill_initial_pages(struct limine_file* file);


