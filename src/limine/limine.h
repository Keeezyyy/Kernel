#pragma once
#include <stdint.h>
#include "./types/limine_types.h"
#include "../x86-64/stdio/stdio.h"



#define LIMINE_MEMMAP_USABLE 0
#define LIMINE_MEMMAP_RESERVED 1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE 2
#define LIMINE_MEMMAP_ACPI_NVS 3
#define LIMINE_MEMMAP_BAD_MEMORY 4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_EXECUTABLE_AND_MODULES 6
#define LIMINE_MEMMAP_FRAMEBUFFER 7
#define LIMINE_MEMMAP_ACPI_TABLES 8

#define NULL ((void *)0)
struct limine_base_revision {
    uint64_t magic0;
    uint64_t magic1;
    uint64_t revision;
};


void request();

struct limine_hhdm_response *getHHDM();
struct limine_framebuffer *getFB();
