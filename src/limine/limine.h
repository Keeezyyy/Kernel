#pragma once
#include <stdint.h>
#include "./framebuffer/framebuffer.h"
#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

#define LIMINE_MEMMAP_REQUEST_ID {LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62}

#define LIMINE_FRAMEBUFFER_REQUEST_ID {LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b}

#define LIMINE_MEMMAP_USABLE 0
#define LIMINE_MEMMAP_RESERVED 1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE 2
#define LIMINE_MEMMAP_ACPI_NVS 3
#define LIMINE_MEMMAP_BAD_MEMORY 4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_EXECUTABLE_AND_MODULES 6
#define LIMINE_MEMMAP_FRAMEBUFFER 7
#define LIMINE_MEMMAP_ACPI_TABLES 8

#define NULL ((void*)0)

typedef struct
{
    uint64_t base;
    uint64_t length;
    uint64_t type;
} limine_memmap_entry;

typedef struct
{
    uint64_t revision;
    uint64_t entry_count;
     limine_memmap_entry **entries;
} limine_memmap_response;

typedef struct
{
    uint64_t revision;
    uint64_t framebuffer_count;
     limine_framebuffer **framebuffers;
} limine_framebuffer_response;

void request();

limine_memmap_response *getMMAP();
limine_framebuffer *getFB();