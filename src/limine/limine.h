#pragma once
#include <stdint.h>
#include "./types/limine_types.h"
#include "../x86-64/stdio/stdio.h"

#define NULL ((void *)0)

struct limine_base_revision {
    uint64_t magic0;
    uint64_t magic1;
    uint64_t revision;
};


void request();

struct limine_hhdm_response *getHHDM();
struct limine_framebuffer *getFB();
struct limine_memmap_response *getMEMMAP();
struct limine_executable_address_response *getEXEC_ADDRESS();
struct limine_executable_file_response *getEXEC_FILE();

