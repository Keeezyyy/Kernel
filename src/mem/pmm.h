#pragma once
#include "bootboot.h"
#include <stdint.h>

#define BITMAP_SIZE 988142 // 4gb of ram (testing number)
#define PHYSICAL_FRAME_SIZE 0x1000

int pmm_alloc(uint64_t *adr);
int pmm_free(uint64_t adr);

void init_pmm(void);
