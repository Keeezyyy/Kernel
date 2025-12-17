#pragma once
#include <stdint.h>
#include "./pmm.h"
#include "../stdio/stdio.h"
#include "../../limine/types/limine_types.h"
#include "../../kernel/kernel.h"
#include "../../limine/limine.h"
#include "../../utils/utils.h"

#define PTE_P (1ULL << 0)
#define PTE_RW (1ULL << 1)
#define PTE_A (1ULL << 5)
#define PTE_D (1ULL << 6)

typedef struct
{
  uint8_t present;
  uint8_t rw;
  uint8_t us;
  uint8_t pwt;
  uint8_t pcd;
  uint8_t accessed;
  uint8_t dirty;
  uint8_t ps;
  uint8_t global;
  uint8_t avl;
  uint64_t phys_addr;
  uint8_t nx;
} pte_params;
#define PTE_WRITE 1

typedef struct
{
  uint16_t pml4_index;
  uint16_t pdpt_index;
  uint16_t pd_index;
  uint16_t pt_index;
} parsed_virtual_address;

typedef struct request_return request_return;
uint64_t init_pml4();
void finilize_new_pml4();
void fill_upper_level(parsed_virtual_address parsed, uint8_t offset);
void clear();
uint64_t get_pte_physical_addresse_from_virtual_address(uint64_t address, uint64_t base_virtual, uint64_t base_physical);
uint64_t make_pte(pte_params params);
parsed_virtual_address parse_virtal_address(uint64_t address);
uint64_t build_virtual_address(parsed_virtual_address addr);

extern void set_cr3(void *);
