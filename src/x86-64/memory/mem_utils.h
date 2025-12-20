#pragma once
#include <stdint.h>

//PTE
//----------------------------------------

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
} page_table_params;
#define PTE_WRITE 1


// pte de-and encoding 
uint64_t make_pte_utils(page_table_params params);
void decode_pte_utils(uint64_t entry, page_table_params *p);
//----------------------------------------
