#include "./memory.h"

extern void kernel_main();

uint64_t PML4[512] __attribute__((aligned(4096)));
uint64_t PDPT[512] __attribute__((aligned(4096)));
uint64_t PD[512] __attribute__((aligned(4096)));
uint64_t PT[512] __attribute__((aligned(4096)));

uint8_t PHY_PAGES_BITMAP[512] __attribute__((aligned(4096))); // BTIMAP OF USABELE PAGES^ (16GB)

uint8_t area_count = 0;
uint64_t total_usable_pages = 0;
struct mem_area mmap_areas[128];

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void init_pml4(struct limine_memmap_response *memmap_res, struct limine_hhdm_response *hhdm_res, struct limine_executable_address_response *exec_res, struct limine_executable_file_response *exec_file)
{
  for (int i = 0; i < 512; i++)
  {
    PML4[i] = 0;
    PDPT[i] = 0;
    PD[i] = 0;
    PT[i] = 0;
    PHY_PAGES_BITMAP[i] = 0;

    if (i < 128)
    {
      mmap_areas[i].base = 0;
      mmap_areas[i].top = 0;
    }
  }
  printf("adr of pml4: %p\n", PML4);

  while (1)
  {
    /* code */
  }

  volatile uint64_t val = exec_res->physical_base;
  val++;

  read_memmap_into_buffer(memmap_res, hhdm_res);

  fill_initial_pages(exec_file->executable_file, hhdm_res->offset, exec_res);
}

void fill_initial_pages(struct limine_file *file, uint64_t hhdm_offset, struct limine_executable_address_response *exec_address)
{
  uint64_t entry_address = (uint64_t)((void *)kernel_start);
  uint64_t file_size = kernel_end - kernel_start;
  uint64_t elf_entry_page = (entry_address / 4096) * 4096;
  uint64_t size_ceil = ALIGN_UP_0x1000(file_size) / 4096;

  printf("size : : 0x%p\n", file_size);
  printf("size ceiled : : 0x%p\n", size_ceil);

  printf("entry : : 0x%p\n", entry_address);
  printf("exit : : 0x%p\n", (uint64_t)((void *)kernel_end));
  printf("entry rounded: : 0x%p\n", elf_entry_page);

  printf("entry rounded: : 0x%p\n", get_pte_physical_addresse_from_virtual_address((uint64_t)&PDPT[0], entry_address, exec_address->physical_base & ~0xFFFULL));

  uint64_t phy_entry = (exec_address->physical_base & ~0xFFFULL);
  printf("physical entry address : 0x%p\n", phy_entry);

  while (1)
  {
  }
  for (uint64_t i = 0; i < size_ceil; i++)
  {
    uint64_t phy_address = (exec_address->physical_base & ~0xFFFULL) + (i * 4096);
    uint64_t vir_address = elf_entry_page + (i * 4096);

    // printf("physical entry address : 0x%p\n", phy_address);
    ////printf("virtual entry address : 0x%p\n", vir_address);

    parsed_virtual_address adr = parse_virtal_address(vir_address);
    ////printf("parsed : 0x%x,  0x%x,  0x%x,  0x%x,  0x%x,  \n", adr.pml4_index, adr.pdpt_index, adr.pd_index, adr.pt_index, adr.offset);

    PML4[adr.pml4_index] = make_pte((pte_params){
        .present = 1,
        .rw = PTE_WRITE,
        .us = 1,
        .pwt = 1,
        .pcd = 1,
        .accessed = 1,
        .dirty = 0,
        .ps = 0,
        .global = 1,
        .avl = 0,
        .phys_addr = get_pte_physical_addresse_from_virtual_address((uint64_t)&PDPT[adr.pdpt_index], entry_address, phy_entry),
        .nx = 0});

    PDPT[adr.pdpt_index] = make_pte((pte_params){
        .present = 1,
        .rw = PTE_WRITE,
        .us = 1,
        .pwt = 1,
        .pcd = 1,
        .accessed = 1,
        .dirty = 0,
        .ps = 0,
        .global = 1,
        .avl = 0,
        .phys_addr = get_pte_physical_addresse_from_virtual_address((uint64_t)&PD[adr.pd_index], entry_address, phy_entry),
        .nx = 0});

    PD[adr.pd_index] = make_pte((pte_params){
        .present = 1,
        .rw = PTE_WRITE,
        .us = 1,
        .pwt = 1,
        .pcd = 1,
        .accessed = 1,
        .dirty = 0,
        .ps = 0,
        .global = 1,
        .avl = 0,
        .phys_addr = get_pte_physical_addresse_from_virtual_address((uint64_t)&PT[adr.pt_index], entry_address, phy_entry),
        .nx = 0});

    PT[adr.pt_index] = make_pte((pte_params){
        .present = 1,
        .rw = PTE_WRITE,
        .us = 1,
        .pwt = 1,
        .pcd = 1,
        .accessed = 1,
        .dirty = 0,
        .ps = 0,
        .global = 1,
        .avl = 0,
        .phys_addr = phy_address,
        .nx = 0});
  }

  uint64_t phy_pm4_address = get_pte_physical_addresse_from_virtual_address((uint64_t)PML4, exec_address->virtual_base, exec_address->physical_base);

  printf("phy pm4 tablr : 0x%p\n", phy_pm4_address);
  printf("vir pm4 tablr : 0x%p\n", PML4);
  while (1)
  {
  }
  set_cr3((void *)phy_pm4_address);
  return;
}

uint64_t get_pte_physical_addresse_from_virtual_address(uint64_t address, uint64_t base_virtual, uint64_t base_physical)
{
  // printf("input : 0x%p .  , output: 0x%p\n", address, (address - base_virtual) + base_physical );
  return ((address - base_virtual) + base_physical);
}

void read_memmap_into_buffer(struct limine_memmap_response *memmap_res, struct limine_hhdm_response *hhdm_res)
{
  uint64_t storage_area_count = memmap_res->entry_count;

  printf("count : 0x%x\n", storage_area_count);

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

  printf("totoal pages available : 0x%p\n", total_usable_pages);
}

uint64_t make_pte(pte_params params)
{
  uint64_t entry = 0;

  entry |= ((uint64_t)params.present & 1) << 0;
  entry |= ((uint64_t)params.rw & 1) << 1;
  entry |= ((uint64_t)params.us & 1) << 2;
  entry |= ((uint64_t)params.pwt & 1) << 3;
  entry |= ((uint64_t)params.pcd & 1) << 4;
  entry |= ((uint64_t)params.accessed & 1) << 5;
  entry |= ((uint64_t)params.dirty & 1) << 6;
  entry |= ((uint64_t)params.ps & 1) << 7;
  entry |= ((uint64_t)params.global & 1) << 8;

  entry |= ((uint64_t)params.avl & 0x7) << 9;

  entry |= (params.phys_addr & 0x000FFFFFFFFFF000ULL);

  entry |= ((uint64_t)params.nx & 1) << 63;

  // printf("pte : 0x%p\n", entry);

  return entry;
}

parsed_virtual_address parse_virtal_address(uint64_t address)
{
  parsed_virtual_address res;

  res.offset = address & 0b111111111111;
  res.pt_index = (address >> 12) & 0b111111111;
  res.pd_index = (address >> 21) & 0b111111111;
  res.pdpt_index = (address >> 30) & 0b111111111;
  res.pml4_index = (address >> 39) & 0b111111111;

  return res;
}
