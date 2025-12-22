#include "./memory.h"
extern void kernel_main();

__attribute__((section(".pml4"), aligned(4096)))
uint64_t PML4[512];


__attribute__((section(".pdpt"), aligned(4096)))
uint64_t PDPT[512];
__attribute__((section(".pdpt"), aligned(4096)))
uint64_t NEW_PAGE_AREA_PDPT[512];


__attribute__((section(".pd"), aligned(4096)))
uint64_t PD[512];
__attribute__((section(".pd"), aligned(4096)))
uint64_t NEW_PAGE_AREA_PD[512];


__attribute__((section(".pt"), aligned(4096)))
uint64_t PT[2048];
__attribute__((section(".pt"), aligned(4096)))
uint64_t NEW_PAGE_AREA_PT[512];


constexpr uint32_t INITIAL_TABLE_COUNT = (sizeof(PML4) + sizeof(PDPT) + sizeof(PD) + sizeof(PT)) / sizeof(PML4[0]) / 512;

const uint64_t PML4_START = (uint64_t)&PML4[0];
const uint64_t PT_END = (uint64_t)&PT[sizeof(PT) / sizeof(PT[0])];

uint64_t PAGE_TABLE_AREA_START;
uint64_t PAGE_TABLE_AREA_END;

extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

static parsed_virtual_address heap_line;
static uint32_t last_used_pt_index;
static inline void fill_upper_level(parsed_virtual_address parsed, uint8_t offset)
{
  printf("indieces in the fill method : 0x%p,    0x%p,    0x%p,    0x%p,  \n ", (uint64_t)parsed.pml4_index, (uint64_t)parsed.pdpt_index, (uint64_t)parsed.pd_index, (uint64_t)parsed.pt_index);
  PML4[parsed.pml4_index] = make_pte((pte_params){
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
    .phys_addr = convert_virtual_to_physical((uint64_t)PDPT),
    .nx = 0});

  PDPT[parsed.pdpt_index] = make_pte((pte_params){
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
    .phys_addr = convert_virtual_to_physical((uint64_t)PD),
    .nx = 0});

  uint64_t *offsettet_pt_ptr = &PT[offset * 512];

  PD[parsed.pd_index] = make_pte((pte_params){
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
    .phys_addr = convert_virtual_to_physical((uint64_t)offsettet_pt_ptr),
    .nx = 0});

  printf("pt index: 0x%p\n", (uint64_t)offsettet_pt_ptr);
}

static inline uint64_t make_pte(pte_params params)
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

  res.pt_index = (address >> 12) & 0b111111111;
  res.pd_index = (address >> 21) & 0b111111111;
  res.pdpt_index = (address >> 30) & 0b111111111;
  res.pml4_index = (address >> 39) & 0b111111111;

  return res;
}

uint64_t build_virtual_address(parsed_virtual_address addr)
{
  uint64_t address = 0xFFFF000000000000;

  address |= (addr.pt_index & 0x1FFULL) << 12;
  address |= (addr.pd_index & 0x1FFULL) << 21;
  address |= (addr.pdpt_index & 0x1FFULL) << 30;
  address |= (addr.pml4_index & 0x1FFULL) << 39;

  return address;
}

void table_area_init() {
  printf("PML4 : 0x%p   -    0x%p\n", &PML4[0], &PML4[sizeof(PML4)/sizeof(PML4[0])]);
  printf("PDPT : 0x%p   -    0x%p\n", &PDPT[0], &PDPT[sizeof(PDPT)/sizeof(PDPT[0])]);
  printf("PD : 0x%p   -    0x%p\n", &PD[0], &PD[sizeof(PD)/sizeof(PD[0])]);
  printf("PT : 0x%p   -    0x%p\n", &PT[0], &PT[sizeof(PT)/sizeof(PT[0])]);
  uint64_t pml4_addr   = (uint64_t)PML4;
  uint64_t pt_end_addr = (uint64_t)PT_END;

  PAGE_TABLE_AREA_START =
    pml4_addr < pt_end_addr ? pml4_addr : pt_end_addr;

  PAGE_TABLE_AREA_END =
    pml4_addr > pt_end_addr ? pml4_addr : pt_end_addr;
}

void init_pml4()
{
  clear();
  init_memmap();
  table_area_init();

  const uint64_t entry_address = (uint64_t)((void *)kernel_start);
  const uint64_t file_size = kernel_end - kernel_start;
  const uint64_t elf_entry_page = (entry_address / 4096) * 4096;
  const uint64_t size_ceil = ALIGN_UP_0x1000(file_size) / 4096;

  if (size_ceil > 512)
    kernel_panic("page limit of 512 reached in pm4 init !!!!\n");

  //init new kernel mapping 
  uint64_t phy_entry = (get_executable_address()->physical_base & ~0xFFFULL);

  init_conversion(phy_entry, entry_address);

  parsed_virtual_address indices = parse_virtal_address(entry_address);
  fill_upper_level(parse_virtal_address(entry_address), 0);
  for (int i = 0; i < size_ceil; i++)
  {
    uint64_t phy_tmp = phy_entry + (i * 0x1000);
    uint64_t vir_tmp = entry_address + (i * 0x1000);

    parsed_virtual_address parsed = parse_virtal_address(vir_tmp);

    PT[parsed.pt_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)vir_tmp),
      .nx = 0});
  }

  //init new page table mapping 
  const uint64_t phy_page_table_entry = phy_entry + (PAGE_TABLE_AREA_START - entry_address);
  const uint64_t virtual_page_table_address = (phy_page_table_entry & 0x000FFFFFFFFFF000ULL) + vmm_get_new_page_table_offset();
  const uint64_t page_table_size_in_pages = (PAGE_TABLE_AREA_END - PAGE_TABLE_AREA_START) / 0x1000;
  printf("phy page table entryy : 0x%p\n", phy_page_table_entry);
  printf("virtual start adr  : 0x%p\n", virtual_page_table_address);

  parsed_virtual_address page_table_indices = parse_virtal_address(virtual_page_table_address);
  {
    PML4[page_table_indices.pml4_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)NEW_PAGE_AREA_PDPT),
      .nx = 0});

    NEW_PAGE_AREA_PDPT[page_table_indices.pdpt_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)NEW_PAGE_AREA_PD),
      .nx = 0});

    NEW_PAGE_AREA_PD[page_table_indices.pd_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)NEW_PAGE_AREA_PT),
      .nx = 0});
  }

  for (int i = 0; i < page_table_size_in_pages; i++)
  {
    const uint64_t phy_tmp = phy_page_table_entry + (i * 0x1000);
    const uint64_t vir_tmp = virtual_page_table_address + (i * 0x1000);

    parsed_virtual_address parsed = parse_virtal_address(vir_tmp);

    NEW_PAGE_AREA_PT[parsed.pt_index] = make_pte((pte_params){
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
      .phys_addr = phy_tmp,
      .nx = 0});
  }


  uint64_t new_table_adr = build_virtual_address(page_table_indices);
  printf("new table adt : 0x%p\n", new_table_adr);
}

uint64_t init_framebuffer_mapping(){
  uint64_t byte_size = getByteSize();
  uint64_t phy_address = getPhyAdr();


  uint64_t huge_pages_amount = byte_size / 0x200000;

  parsed_virtual_address parsed = parse_virtal_address((uint64_t)((void *)kernel_start));

  parsed.pd_index += +1;

  heap_line = parsed;

  uint64_t new_fb_vir_address = build_virtual_address(parsed);

  for (int i = 0; i < huge_pages_amount; i++)
  {
    PML4[parsed.pml4_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)PDPT),
      .nx = 0});

    PDPT[parsed.pdpt_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)PD),
      .nx = 0});

    PD[parsed.pd_index++] = make_pte((pte_params){
      .present = 1,
      .rw = PTE_WRITE,
      .us = 1,
      .pwt = 1,
      .pcd = 1,
      .accessed = 1,
      .dirty = 0,
      .ps = 1,
      .global = 1,
      .avl = 0,
      .phys_addr = (uint64_t)phy_address + (i * 0x200000),
      .nx = 0});
  }

  byte_size -= huge_pages_amount * 0x200000;

  uint64_t pages_amount_required = CEIL_DIV(byte_size, 0x1000);
  uint64_t page_table_count = CEIL_DIV(pages_amount_required, 512);

  // printf("page table count : 0x%p\n", page_table_count) ;
  // printf("pages reqired  : 0x%p\n", pages_amount_required) ;

  uint64_t inti_pd_index = parsed.pd_index;

  uint64_t pages_left = pages_amount_required;

  for (int i = 0; i < page_table_count; i++)
  {
    parsed.pd_index = inti_pd_index + i;
    fill_upper_level(parsed, i + 1);

    uint64_t entries_this_pt =
      (pages_left >= 512) ? 512 : pages_left;

    for (int k = 0; k < entries_this_pt; k++)
    {
      uint64_t index_in_pt = parsed.pt_index + k + ((i + 1) * 512);

      // PT[parsed.pt_index+k + ((i+1)*512)] = make_pte((pte_params){
      PT[index_in_pt] = make_pte((pte_params){
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
        .phys_addr = phy_address + (k * 0x1000) + (huge_pages_amount * 0x200000),
        .nx = 0});
      last_used_pt_index = index_in_pt;
    }
    pages_left -= entries_this_pt;
  }

  heap_line = parsed;
  // printf("finished initilizing pml4\n");

  return new_fb_vir_address;
}

void finilize_new_pml4()
{
  set_cr3((void *)convert_virtual_to_physical((uint64_t)PML4));
}

void clear()
{
  for (int i = 0; i < 512; i++)
  {
    PML4[i] = 0;
    PDPT[i] = 0;
    PD[i] = 0;
    PT[i] = 0;
  }
}


