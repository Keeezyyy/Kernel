#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"
#include "../../kernel/kernel.h"
#include "./mem_utils.h"
#include "./pmm.h"

//PTE bits makrso
#define PTE_PRESENT        (1ULL << 0)   // Present
#define PTE_RW             (1ULL << 1)   // Read/Write (1 = writable)
#define PTE_USER           (1ULL << 2)   // User/Supervisor (1 = user)
#define PTE_PWT            (1ULL << 3)   // Page Write Through
#define PTE_PCD            (1ULL << 4)   // Page Cache Disable
#define PTE_ACCESSED       (1ULL << 5)   // Accessed
#define PTE_DIRTY          (1ULL << 6)   // Dirty (PTE only)
#define PTE_PS             (1ULL << 7)   // Page Size (PDE/PDPTE only)
#define PTE_PAT            (1ULL << 7)   // PAT (PTE only)
#define PTE_GLOBAL         (1ULL << 8)   // Global (ignored if CR4.PGE=0)
#define PTE_AVL0           (1ULL << 9)
#define PTE_AVL1           (1ULL << 10)
#define PTE_AVL2           (1ULL << 11)
#define PTE_ADDR_MASK      0x000FFFFFFFFFF000ULL
#define PTE_AVL_HIGH_MASK  (0x7FULL << 52)
#define PTE_PROT_KEY       (1ULL << 59)  // Protection Key (if enabled)
#define PTE_NX             (1ULL << 63)  // No Execute (EFER.NXE=1)

#define PTE_GET_ADDR(pte)   ((pte) & PTE_ADDR_MASK)

#define bool uint8_t
#define true 1
#define false 0

#define MAX_RAM_IN_GB 128
#define GIGABYTE 0x40000000ULL
#define DIRECT_MAPPING_OFFSET 0xffffB00000000000ULL



/*
 *
 *
 *
 *
 *
 * */


//forward decl of memory.h util finctions
//TODO: put these in the uitil file 
typedef struct parsed_virtual_address parsed_virtual_address;
typedef struct pte_params pte_params;
enum virtual_mem_area_enum {
  KERNEL_HEAP,
};
struct vm_area{
  uint64_t start_address;
  uint64_t end_address;
};


void vmm_init();
void* vmm_alloc(uint32_t size, enum virtual_mem_area_enum);
void  vmm_free(void*);

/*CR3 Utils
 *
 * */
static inline uint64_t get_cr3();
static inline void reload_cr3();
static inline void* get_va_from_physical_address(uint64_t physical_address);



uint64_t vmm_get_new_page_table_offset(); // gloabal function to return DIRECT_MAPPING_OFFSET
