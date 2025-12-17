#include "./pmm.h"

static uint64_t PHY_PAGES_BITMAP[16384];

static void* last_searched;

static uint8_t area_count = 0;
static uint64_t total_usable_pages = 0;
static struct mem_area mmap_areas[128];

void init_memmap()
{

  struct limine_memmap_response *memmap_res = get_memmap();
  struct limine_hhdm_response *hhdm_res = getHHDM();

  uint64_t storage_area_count = memmap_res->entry_count;

  struct limine_memmap_entry *areas = *memmap_res->entries;

  uint8_t valid_area_count = 0;
  uint64_t total_pages = 0;

  for (uint64_t i = 0; i < storage_area_count; i++)
  {
    struct limine_memmap_entry entry = areas[i];


    //printf("type : 0x%p\n", entry.type);
    if (entry.type == LIMINE_MEMMAP_USABLE || entry.type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
    {
      uint64_t rounded_floor = (entry.base / 4096) * 4096;
      uint64_t rounded_top = ALIGN_UP_0x1000(entry.base + entry.length);


      mmap_areas[valid_area_count].top = rounded_top;
      mmap_areas[valid_area_count].base = rounded_floor;
      mmap_areas[valid_area_count].page_length = (rounded_top - rounded_floor)/0x1000;
      printf("current area : 0x%p,    0x%p,   type : 0x%p, length : 0x%p \n", rounded_floor, rounded_top, entry.type, mmap_areas[valid_area_count].page_length);
      valid_area_count++;

      total_pages += rounded_top - rounded_floor;
    }
  }

  area_count = valid_area_count;
  total_usable_pages = total_pages / 4096;
}

uint32_t getBitmapIndex(uint64_t physical_address)
{
  uint32_t page_count = 0;
  for (int i = 0; i < 128; i++)
  {
    struct mem_area current_area = mmap_areas[i];
    if (current_area.base == 0 && current_area.top == 0)
      break;

    printf("current area : 0x%p,    0x%p, pahe count : 0x%p, physocla : 0x%p\n", current_area.base, current_area.top, page_count/4096, physical_address);
    if (physical_address >= current_area.base && physical_address <= current_area.top)
    {
      uint64_t rounded_floor = (physical_address / 4096) * 4096;
      page_count += current_area.top - rounded_floor;
      printf("current area : 0x%p,    0x%p, pahe count : 0x%p\n", current_area.base, current_area.top, page_count);
    }
    page_count += current_area.top - current_area.base;
  }
}

void claim_page(uint64_t page)
{
  uint32_t byte = page / 8;
  uint32_t bit = page % 8;

  PHY_PAGES_BITMAP[byte] |= (1 << bit);
}

uint8_t getIndexInU64(uint64_t val){
  for(int i = 0; i<64 ; i++){
    if(((val >> i) & 1) == 0)
      return i;
  }
}

uint64_t convert_index_to_address(uint32_t index){
  uint64_t pages_tested = 0;


  for(int i = 0; i< area_count; i++){
    struct mem_area current_area = mmap_areas[i];

    uint64_t top_pages = pages_tested + current_area.page_length;
    uint64_t bot_pages = pages_tested;


    if(index >= bot_pages && index < top_pages){
      uint32_t offset_in_area = index - bot_pages;

      return current_area.base + (offset_in_area * 0x1000);
    }else {
      pages_tested += current_area.page_length;
    }
  }
}

void set_bit_in_integral(uint8_t index, uint64_t *val){
  *val |= (1ULL << index);
}
void reset_bit_in_integral(uint8_t index, uint64_t *val){
  *val &= ~(1ULL << index);
}


phys_addr_t pmm_alloc_frame(){

  uint8_t index_in_val = 0;
  uint64_t long_index = 0;

  for(int i = 0; i< sizeof(PHY_PAGES_BITMAP) / sizeof(PHY_PAGES_BITMAP[0]); i++){
    if(PHY_PAGES_BITMAP[i] == 0xFFFFFFFFFFFFFFFF)
      continue;


    index_in_val = getIndexInU64(PHY_PAGES_BITMAP[i]);
    long_index = i;
    set_bit_in_integral(index_in_val, &PHY_PAGES_BITMAP[i]);
    break;
  }

  const uint64_t total_index = (long_index * 64) + index_in_val;
  return convert_index_to_address(total_index);
}

//TODO: add variable in mem_area to track pages so far to increase free ans malloc performance

uint32_t convert_physical_to_bitmap_index(uint64_t physical_address){
  uint64_t rounded_pages = (physical_address / 0x1000);

  uint64_t pages_counted = 0;
 
  for(int i = 0; i< area_count; i++){
    struct mem_area current_area = mmap_areas[i];

    if(physical_address >= current_area.base && physical_address < current_area.top){
      return pages_counted + ((physical_address - current_area.base) / 0x1000);
    }else{
      pages_counted += current_area.page_length;
    }

  }
}

void pmm_free_frame(phys_addr_t adr){
    uint64_t index = convert_physical_to_bitmap_index(adr);
    reset_bit_in_integral(index % 64, &PHY_PAGES_BITMAP[index / 64]);
}
