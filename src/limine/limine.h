#pragma once
#include <stdint.h>
#include "./types/limine_types.h"
#include "../x86-64/stdio/stdio.h"
#include "../x86-64/memory/memory.h"

#define NULL ((void *)0)

struct limine_base_revision
{
    uint64_t magic0;
    uint64_t magic1;
    uint64_t revision;
};

typedef struct request_return
{
    struct limine_framebuffer *fb;
    struct limine_memmap_response *memmap;
    struct limine_executable_address_response *exe_address;
    struct limine_executable_file_response *exe_fil;
    struct limine_hhdm_response *hhdm;
} request_return;



request_return *request(void);

struct limine_memmap_response *get_memmap(void);
struct limine_hhdm_response *get_hhdm(void);
struct limine_executable_address_response *get_executable_address(void);
struct limine_framebuffer *get_framebuffer(void);
struct limine_executable_file_response *get_executable_file(void);

uint64_t getHHDMOffset();

struct limine_hhdm_response *getHHDM();
uint64_t getByteSize();
uint64_t getPhyAdr();
