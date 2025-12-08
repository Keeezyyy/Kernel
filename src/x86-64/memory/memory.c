
#include "./memory.h"

static uint64_t PML4[512] __attribute__((aligned(4096)));
static uint64_t PDPT[512]__attribute__((aligned(4096)));
static uint64_t PD[512]__attribute__((aligned(4096)));
static uint64_t PT[512]__attribute__((aligned(4096)));

static uint8_t PHY_PAGES_BITMAP[512]; //BTIMAP OF USABELE PAGES^ (16GB)

static uint8_t area_count = 0;
static uint64_t total_usable_pages = 0;
static struct mem_area mmap_areas[128];

void init_pml4(struct limine_memmap_response * memmap_res, struct limine_hhdm_response * hhdm_res){
  for(int i = 0; i<512; i++){
    PML4[i] = 0;
    PDPT[i] = 0;
    PD[i] = 0;
    PT[i] = 0;
  }


  read_memmap_into_buffer(memmap_res, hhdm_res);
}

void read_memmap_into_buffer(struct limine_memmap_response * memmap_res, struct limine_hhdm_response * hhdm_res){
  uint64_t storage_area_count = memmap_res->entry_count;

  printf("count : 0x%x\n", storage_area_count);

  struct limine_memmap_entry* areas = *memmap_res->entries;

  uint8_t valid_area_count = 0;
  uint64_t total_pages = 0;

  for(uint64_t i = 0; i<storage_area_count; i++){
    struct limine_memmap_entry entry = areas[i];

    if(entry.type == LIMINE_MEMMAP_USABLE){
      uint64_t rounded_floor = (entry.base / 4096) * 4096;
      uint64_t rounded_top = ALIGN_CEIL((entry.base + entry.length), 4096);

      mmap_areas[valid_area_count].top = rounded_top;
      mmap_areas[valid_area_count].base = rounded_floor;
      valid_area_count++;

      printf("base : 0x%p\n", rounded_floor);
      printf("length : 0x%p\n", rounded_top);


      total_pages += rounded_top - rounded_floor; 
    }
  }

  area_count = valid_area_count;
  total_usable_pages = total_pages;
  printf("area count  : 0x%p\n", area_count);
  printf("total pages: 0x%p\n", total_pages);
}
