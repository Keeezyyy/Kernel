#include "./bitmap.h"

static uint8_t PHY_PAGES_BITMAP[512] __attribute__((aligned(4096))); // BTIMAP OF USABELE PAGES^ (16GB)

static uint8_t area_count = 0;
static uint64_t total_usable_pages = 0;
static struct mem_area mmap_areas[128];

void init_memmap(struct limine_memmap_response *memmap_res, struct limine_hhdm_response *hhdm_res){
  for(int i = 0; i<512; i++){

    PHY_PAGES_BITMAP[i] = 0;

    if(i<128)
      mmap_areas[i].base = 0;
      mmap_areas[i].top = 0;
  }


  uint64_t storage_area_count = memmap_res->entry_count;

  struct limine_memmap_entry *areas = *memmap_res->entries;

  uint8_t valid_area_count = 0;
  uint64_t total_pages = 0;

  for (uint64_t i = 0; i < storage_area_count; i++)
  {
    struct limine_memmap_entry entry = areas[i];

    if (entry.type == LIMINE_MEMMAP_USABLE)
    {
      uint64_t rounded_floor = (entry.base / 4096) * 4096;
      uint64_t rounded_top = ALIGN_UP_0x1000(entry.base + entry.length);

      mmap_areas[valid_area_count].top = rounded_top;
      mmap_areas[valid_area_count].base = rounded_floor;
      valid_area_count++;

      total_pages += rounded_top - rounded_floor;
    }
  }

  area_count = valid_area_count;
  total_usable_pages = total_pages / 4096;
}


void claim_page(uint64_t page){
  uint32_t byte = page / 8;
  uint32_t bit  = page % 8;

  PHY_PAGES_BITMAP[byte] |= (1 << bit);
}
