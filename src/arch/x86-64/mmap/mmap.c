#include "mmap.h"
#include "debug/debug.h"

static u64 usable_ram = 0;

void init_mmap(BOOTBOOT *boot_info) {
  MMapEnt *mmap = &boot_info->mmap;

  u32 num_mmap_entries = (boot_info->size - 128) / 16;

  for (int i = 0; i < num_mmap_entries; i++) {
    usable_ram += mmap[i].size;

    debug_printf("[MMAP] : Memory area :adr : 0x%016llx, size : 0x%016llx\n",
                 (unsigned long long)mmap[i].ptr,
                 (unsigned long long)mmap[i].size);
  }

  debug_printf("[MMAP] : usable ram %d byte\n",
               usable_ram);
}
