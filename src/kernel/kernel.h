#pragma once
#include <stdint.h>

struct kernel_region {
    uint64_t start;
    uint64_t end;
    uint32_t size;
};

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

struct kernel_region get_kernel_region(void);

extern void setStack(void*);

