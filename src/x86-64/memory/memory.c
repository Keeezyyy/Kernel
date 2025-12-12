#include "./memory.h"
extern void kernel_main();

static uint64_t PML4[512] __attribute__((aligned(4096)));
static uint64_t PDPT[512] __attribute__((aligned(4096)));
static uint64_t PD[512] __attribute__((aligned(4096)));
static uint64_t PT[512] __attribute__((aligned(4096)));
extern uint8_t kernel_start[];
extern uint8_t kernel_end[];

void init_pml4()
{

  clear();
  
  uint64_t entry_address = (uint64_t)((void *)kernel_start);
  uint64_t file_size = kernel_end - kernel_start;
  uint64_t elf_entry_page = (entry_address / 4096) * 4096;
  uint64_t size_ceil = ALIGN_UP_0x1000(file_size) / 4096;
  
  uint64_t phy_entry = (get_executable_address()->physical_base & ~0xFFFULL);
  printf("0x%p\n", phy_entry);
  printf("0x%p\n", entry_address);
  
  init_conversion(phy_entry, entry_address);

  fill_upper_level(parse_virtal_address((uint64_t)((void *)kernel_start)));

  printf("0x%p\n", convert_virtual_to_physical((uint64_t)PML4));

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

  printf("finished initilizing pml4\n");
}

void finilize_new_pml4(){
    set_cr3((void *)convert_virtual_to_physical((uint64_t)PML4));
}

void clear()
{
  uint64_t *correct = (uint64_t *)0xffff800007ed2000;

  for (int i = 0; i < 512; i++)
  {
    PML4[i] = 0;
    PDPT[i] = 0;
    PD[i] = 0;
    PT[i] = 0;
  }
}

void fill_upper_level(parsed_virtual_address parsed)
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

  PD[parsed.pt_index] = make_pte((pte_params){
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
      .phys_addr = convert_virtual_to_physical((uint64_t)PT),
      .nx = 0});

  printf("pml4 : 0x%p\n", PML4[parsed.pml4_index]);
  printf("pdpt : 0x%p\n", PDPT[parsed.pdpt_index]);
  printf("pt : 0x%p\n", PD[parsed.pt_index]);
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

  res.pt_index = (address >> 12) & 0b111111111;
  res.pd_index = (address >> 21) & 0b111111111;
  res.pdpt_index = (address >> 30) & 0b111111111;
  res.pml4_index = (address >> 39) & 0b111111111;

  return res;
}

uint64_t build_virtual_address(parsed_virtual_address addr)
{
  uint64_t address = 0;

  address |= (addr.pt_index & 0x1FFULL) << 12;
  address |= (addr.pd_index & 0x1FFULL) << 21;
  address |= (addr.pdpt_index & 0x1FFULL) << 30;
  address |= (addr.pml4_index & 0x1FFULL) << 39;

  return address;
}

table_indices find_empty_slot()
{
  table_indices res;
  res.buffer = 0xFFFFFFFFFFFFFFFF;
  for (int i = 0; i < 512; i++)
  {

    if (!(PML4[i] & 1) && res.indices.pml4_index == 0xFFFF)
      res.indices.pml4_index = i;

    if (!(PDPT[i] & 1) && res.indices.pdpt_index == 0xFFFF)
      res.indices.pdpt_index = i;

    if (!(PD[i] & 1) && res.indices.pd_index == 0xFFFF)
      res.indices.pd_index = i;

    if (!(PT[i] & 1) && res.indices.pt_index == 0xFFFF)
      res.indices.pt_index = i;
  }

  return res;
}

void *malloc_physical_address(uint64_t physical_address, uint64_t length)
{

  printf("phy : 0x%p,     0x%p\n", physical_address, length);

  while (1)
  {
    /* code */
  }
  

  parsed_virtual_address indices = find_empty_slot().indices;

  printf("values : 0x%p,    0x%p,    0x%p,    0x%p,  \n ", (uint64_t)indices.pml4_index, (uint64_t)indices.pdpt_index, (uint64_t)indices.pd_index, (uint64_t)indices.pt_index);

  fill_upper_level(indices);

  for (int i = 0; i < length; i++)
  {
    PT[indices.pt_index] = make_pte((pte_params){
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
        .phys_addr = physical_address + (i * 0x1000),
        .nx = 0});
  }

  return (void*)build_virtual_address(indices);
}
