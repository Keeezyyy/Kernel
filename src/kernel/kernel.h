#pragma once
#include <stdint.h>
#include "../x86-64/stdio/stdio.h"

struct kernel_region {
    uint64_t start;
    uint64_t end;
    uint32_t size;
};

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

struct kernel_region get_kernel_region(void);
uint64_t convert_virtual_to_physical(uint64_t virtual_adr);
uint64_t convert_physical_to_virtual(uint64_t physical_adr);
void init_conversion(uint64_t phy, uint64_t vir);

void cpuid(uint32_t leaf,uint32_t* eax,uint32_t* ebx, uint32_t* ecx, uint32_t* edx);
void kernel_panic(char*);
void enable_irq(void);

extern void setStack(void*);
extern void kernel_halt(void);



