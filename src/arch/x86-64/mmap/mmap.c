#include "mmap.h"
#include "debug/debug.h"
#include "mem/pmm.h"

static u64 usable_ram = 0;

static memory_map_t mmap;

void init_mmap(BOOTBOOT *boot_info) {
  mmap.ptr = &boot_info->mmap;
  mmap.entries = (boot_info->size - 128) / sizeof(MMapEnt);

  for (u64 i = 0; i < mmap.entries; i++) {
    MMapEnt *e = &mmap.ptr[i];

    if (MMapEnt_IsFree(e))
      usable_ram += MMapEnt_Size(e);
  }

  debug_printf("[MMAP] : usable ram %llu byte\n",
               (unsigned long long)usable_ram);
}

static bool was_used = false;
memory_map_t *get_memory_map(void) {
  if (mmap.ptr == 0)
    panic("memory map requested before init_mmap()\n");

  if (was_used)
    panic("memory map was requested twice it should only be requested once by pmm.c\n");

  was_used = true;
  return &mmap;
}
