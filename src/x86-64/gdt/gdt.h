#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"

#define GDT_KERNEL_CODE_ACCESS    0x9A
#define GDT_KERNEL_CODE_FLAGS     0x20   // L-bit = 1 (64-bit), D-bit = 0

#define GDT_KERNEL_DATA_ACCESS    0x92
#define GDT_KERNEL_DATA_FLAGS     0x00   // Data segments ignore L/D bits


#define GDT_USER_CODE_ACCESS     0xFA
#define GDT_USER_CODE_FLAGS      0x20

#define GDT_USER_DATA_ACCESS     0xF2
#define GDT_USER_DATA_FLAGS      0x00


typedef struct  {
    uint32_t reserved0;

    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;

    uint64_t reserved1;

    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;

    uint64_t reserved2;

    uint16_t io_map_base;
} __attribute__((packed)) tss_entry;

typedef struct {
    uint16_t limit_low;       // Bits 0–15 des Segmentlimits
    uint16_t base_low;        // Bits 0–15 der Basisadresse
    uint8_t  base_middle;     // Bits 16–23 der Basisadresse
    uint8_t  access;          // Zugriffsfeld (Access Byte)
    uint8_t  granularity;     // Flags + Bits 16–19 des Limits
    uint8_t  base_high;       // Bits 24–31 der Basisadresse
} __attribute__((packed)) gdt_entry_t;


typedef struct {
    uint16_t limit;   // Größe der GDT - 1
    uint64_t base;    // Adresse der GDT
} __attribute__((packed)) gdt_ptr_t;

__attribute__((cdecl)) void load_gdtr(void* gdtr);

void gdt_set_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

void init_gdt();

void gdt_set_tss(int idx, uint64_t base, uint32_t limit);

