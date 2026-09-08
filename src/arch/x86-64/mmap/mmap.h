#pragma once

#include "bootboot.h"

typedef struct {
  MMapEnt *ptr;
  u64 entries;
} memory_map_t;

void init_mmap(BOOTBOOT *boot_info);

memory_map_t *get_memory_map(void);
