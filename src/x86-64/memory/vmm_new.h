#pragma once
#include <stdint.h>

#define DIRECT_MAPPING_OFFSET 0xffffB00000000000ULL

void vmm_init();

void* vmm_alloc(uint32_t size);
void  vmm_free(void*);



/*CR3 Utils
 *
 * */
static inline uint64_t get_cr3();
static inline void reload_cr3();
static inline void* get_va_from_physical_address(uint64_t physical_address);
