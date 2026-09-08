#include "pmm.h"
#include "arch/x86-64/mmap/mmap.h"
#include "config.h"
#include "debug/debug.h"
#include <stdint.h>

static u8 physical_frame_bitmap[BITMAP_SIZE];

memory_map_t *mem_map;

static int _get_physical_adr_from_bit(u64 bit_index, u64 *phy_adr) {
  if (!phy_adr)
    return 1;

  MMapEnt *ents = (MMapEnt *)mem_map->ptr;

  for (u64 i = 0; i < mem_map->entries; i++) {
    MMapEnt *m = &ents[i];

    if (!MMapEnt_IsFree(m) || MMapEnt_Size(m) == 0)
      continue;

    u64 start = m->ptr;
    u64 end = m->ptr + MMapEnt_Size(m);
    if (end <= start)
      continue;

    u64 frames = (end - start) / PHYSICAL_FRAME_SIZE;
    if (bit_index < frames) {
      *phy_adr = start + bit_index * PHYSICAL_FRAME_SIZE;
      return 0;
    }
    bit_index -= frames;
  }

  return 1;
}

static u64 _get_adr_from_bit_idx(uint64_t adr) {
  MMapEnt *ents = (MMapEnt *)mem_map->ptr;

  u64 floored_adr = FLOOR_TO(adr, PHYSICAL_FRAME_SIZE);

  u64 bit_index = 0;

  for (u64 i = 0; i < mem_map->entries; i++) {
    MMapEnt *m = &ents[i];

    if (!MMapEnt_IsFree(m) || MMapEnt_Size(m) == 0)
      continue;

    u64 start = m->ptr;
    u64 end = m->ptr + MMapEnt_Size(m);
    if (start < floored_adr || end >= floored_adr) {

      bit_index += (end - start) / PHYSICAL_FRAME_SIZE;
      continue;
    } else {
      bit_index += (floored_adr - start) / PHYSICAL_FRAME_SIZE;

      u64 idx = bit_index / 8;
      u64 offset = bit_index % 8;

      debug_printf("byte : %d\n", physical_frame_bitmap[idx]);

      if (BIT_CHECK(physical_frame_bitmap[idx], offset) != 1) {
        // frame was already freed

        debug_printf("[pmm] : tried freeing adr : 0x%016llx was already free \n", adr);
        return 1;
      } else {
        BIT_TOGGLE(physical_frame_bitmap[idx], offset);
        return 0;
      }
    }
  }
  return 1;
}

int pmm_alloc(uint64_t *adr) {
  const size_t words = BITMAP_SIZE / sizeof(u64);
  u64 *bitmap = (u64 *)physical_frame_bitmap;

  for (size_t i = 0; i < words; i++) {
    if (bitmap[i] == UINT64_MAX)
      continue;

    unsigned j = (unsigned)__builtin_ctzll(~bitmap[i]);

    if (_get_physical_adr_from_bit(i * 64 + j, adr))
      return 1;

    bitmap[i] |= BIT_SET(bitmap[i], j);
    return 0;
  }
  return 1;
}

int pmm_free(uint64_t adr) {
  MMapEnt *ents = (MMapEnt *)mem_map->ptr;

  u64 floored_adr = FLOOR_TO(adr, PHYSICAL_FRAME_SIZE);

  u64 bit_index = 0;

  for (u64 i = 0; i < mem_map->entries; i++) {
    MMapEnt *m = &ents[i];

    if (!MMapEnt_IsFree(m) || MMapEnt_Size(m) == 0)
      continue;

    u64 start = m->ptr;
    u64 end = m->ptr + MMapEnt_Size(m);
    if (start > floored_adr || end <= floored_adr) {

      bit_index += (end - start) / PHYSICAL_FRAME_SIZE;
      continue;
    } else {
      bit_index += (floored_adr - start) / PHYSICAL_FRAME_SIZE;

      u64 idx = bit_index / 8;
      u64 offset = bit_index % 8;

      debug_printf("byte : %d\n", physical_frame_bitmap[idx]);

      if (BIT_CHECK(physical_frame_bitmap[idx], offset) != 1) {
        // frame was already freed

        debug_printf("[pmm] : tried freeing adr : 0x%016llx was already free \n", adr);
        return 1;
      } else {
        BIT_TOGGLE(physical_frame_bitmap[idx], offset);
        return 0;
      }
    }
  }
  return 1;
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
    debug_printf("[PMM] : Memory area :adr : 0x%016llx -  0x%016llx,  size : 0x%016llx, type : %d\n",
                 (unsigned long long)mem_map->ptr[i].ptr,
                 (unsigned long long)mem_map->ptr[i].ptr + (unsigned long long)mem_map->ptr[i].size & 0xFFFFFFFFFFFFFFF0,
                 (unsigned long long)mem_map->ptr[i].size & 0xFFFFFFFFFFFFFFF0,
                 (unsigned long long)mem_map->ptr[i].size & 0xF);
  }

  debug_printf("entries count : %d\n", mem_map->entries);
}
