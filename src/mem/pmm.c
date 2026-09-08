#include "pmm.h"
#include "arch/x86-64/mmap/mmap.h"
#include "config.h"
#include "debug/debug.h"

static u8 physical_frame_bitmap[BITMAP_SIZE];

memory_map_t *mem_map;

int pmm_alloc(void **adr, uint32_t size) {
  return 0;
}

// TODO: find a way to fix fragmantation
void init_pmm(void) {
  mem_map = get_memory_map();

  for (u32 i = 0; i < mem_map->entries; i++) {
    MMapEnt *m = &((MMapEnt *)mem_map->ptr)[i];

    m->ptr = ROUND_UP((u64)m->ptr, PHYSICAL_FRAME_SIZE);

    u64 type = MMapEnt_Type(m);
    u64 start = ROUND_UP(m->ptr, PHYSICAL_FRAME_SIZE);
    u64 end = FLOOR_TO(m->ptr + MMapEnt_Size(m), PHYSICAL_FRAME_SIZE);

    if (end <= start) {
      m->size = MMAP_USED;
    } else {
      m->ptr = start;
      m->size = (end - start) | type;
    }
    debug_printf("[PMM] : Memory area :adr : 0x%016llx,  size : 0x%016llx, type : %d\n",
                 (unsigned long long)mem_map->ptr[i].ptr,
                 (unsigned long long)mem_map->ptr[i].size & 0xFFFFFFFFFFFFFFF0,
                 (unsigned long long)mem_map->ptr[i].size & 0xF);
  }
}
