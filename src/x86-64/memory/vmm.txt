#include "./vmm.h"


static uint64_t* swap_pt_entry;
static uint64_t swap_physical_page;
static parsed_virtual_address swap_indices;
//local utils
static inline uint64_t *pte_to_table_virt(uint64_t entry)
{
  uint64_t phys = parse_physical_address_from_page_table_entry(entry);
  return (uint64_t *)convert_paging_phys_to_vir(phys);
}

static inline uint64_t alloc_zero_table_page(void)
{
  uint64_t phys = pmm_alloc_frame();
  printf("new physical page : 0x%p\n", phys);
  if (!phys)
    return 0;
  uint64_t *vir = convert_paging_phys_to_vir(phys);
  return phys;
}

static inline uint64_t make_table_pte(uint64_t phys)
{
  return construct_pte((pte_params){
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
    .phys_addr = phys,
    .nx = 0});
}

static inline uint64_t get_cr3()
{
  uint64_t cr3 = 0;
  asm("mov %%cr3, %0\n" : "=r"(cr3));
  return cr3;
}

static inline void reload_cr3(){
     asm volatile (
        "mov %%cr3, %%rax\n"
        "mov %%rax, %%cr3\n"
        :
        :
        : "rax", "memory"
    );
}

inline uint64_t construct_pte(pte_params params)
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
//...........................................


//swap page 
void init_swap_page(){
  struct vm_area area = get_area((enum area_type)1);

  printf("area start : 0x%p\n", area.start_address);
  printf("area end : 0x%p\n", area.end_address);

  const parsed_virtual_address starting_indices = parse_virtal_address(area.start_address);
  const parsed_virtual_address ending_indices = parse_virtal_address(area.end_address);

  const uint64_t phy_cr3 = get_cr3();
  const uint64_t *pml4_ptr = (uint64_t *)convert_paging_phys_to_vir(phy_cr3);

  uint64_t pml4_val = get_present_slot_in_table(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index);
  if (pml4_val == 0)
    kernel_panic("no page table for vmm in pml4");

  const parsed_virtual_address slot_indices = get_small_page_slot(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index, SWAP_PAGE_LENGTH );

  printf("pml4 : 0x%p, pdpt : 0x%p, pd : 0x%p, pt : 0x%p\n", slot_indices.pml4_index, slot_indices.pdpt_index, slot_indices.pd_index, slot_indices.pt_index);

  fill_empty_memory_slot(slot_indices, SWAP_PAGE_LENGTH);

  uint64_t address = build_virtual_address(slot_indices);

  uint64_t *pml4, *pdpt, *pd, *pt;
  parse_indices_to_pointer(slot_indices, &pml4, &pdpt, &pd, &pt);

  swap_indices = slot_indices;
  swap_pt_entry = pt;
}
void init_vmm(){
  printf("doing something \n");
  init_swap_page();
}
void *load_physical_page_into_swap_page(uint64_t physical_page_address){
  *swap_pt_entry = make_table_pte(physical_page_address);
  uint64_t address = build_virtual_address(swap_indices);
  return (void*)address;
}
void unmap_swap(){
  *swap_pt_entry = make_table_pte(0x1000);
}
//........................................


// TODO: use searhcing algorithm with better preformance
parsed_virtual_address find_virtual_memory_slot(struct vm_area *area, uint32_t page_length)
{
  const parsed_virtual_address starting_indices = parse_virtal_address(area->start_address);
  const parsed_virtual_address ending_indices = parse_virtal_address(area->end_address);


  const uint64_t phy_cr3 = get_cr3();
  const uint64_t *pml4_ptr = (uint64_t *)convert_paging_phys_to_vir(phy_cr3);

  uint64_t pml4_val = get_present_slot_in_table(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index);
  if (pml4_val == 0)
    kernel_panic("no page table for vmm in pml4");

  const parsed_virtual_address slot_indices = get_small_page_slot(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index, page_length);

 
  //treat the 4x 16 bit in slot indices as one 64 bit uint 
  if(*((uint64_t*)&slot_indices) == 0){
    printf("no slot fount have to create new \n");
    return alloc_new_mapping_slot(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index);
  }

  return slot_indices;
}

// TODO: search for overlapping contiguous pages too
const parsed_virtual_address get_small_page_slot(uint64_t *pml4, uint16_t starting_index, uint16_t ending_index, uint32_t length)
{
  parsed_virtual_address result = {0};

  uint64_t *pt_start = NULL;
  uint32_t contiguous_pages = 0;

  for (uint16_t pml4_i = starting_index; pml4_i <= ending_index; pml4_i++)
  {

    if (!(pml4[pml4_i] & 0x1))
      continue;

    uint64_t phys_pdpt = parse_physical_address_from_page_table_entry(pml4[pml4_i]);
    uint64_t *pdpt = convert_paging_phys_to_vir(phys_pdpt);

    for (uint16_t pdpt_i = 0; pdpt_i < 512; pdpt_i++)
    {

      if (!(pdpt[pdpt_i] & 0x1))
        continue;

      uint64_t phys_pd = parse_physical_address_from_page_table_entry(pdpt[pdpt_i]);
      uint64_t *pd = convert_paging_phys_to_vir(phys_pd);

      for (uint16_t pd_i = 0; pd_i < 512; pd_i++)
      {

        if (!(pd[pd_i] & 0x1))
          continue;

        uint64_t phys_pt = parse_physical_address_from_page_table_entry(pd[pd_i]);
        uint64_t *pt = convert_paging_phys_to_vir(phys_pt);

        contiguous_pages = find_contiguous_empty_slots(
          pt,
          &pt_start,
          pt_start,
          contiguous_pages,
          length);

        if (contiguous_pages == length && pt_start)
        {
          result.pml4_index = pml4_i;
          result.pdpt_index = pdpt_i;
          result.pd_index = pd_i;
          result.pt_index = (uint16_t)(pt_start - pt);
          return result;
        }
      }
    }
  }

  return result;
}

void parse_indices_to_pointer(parsed_virtual_address indices, uint64_t **out_pml4, uint64_t **out_pdpt, uint64_t **out_pd, uint64_t **out_pt)
{
  const uint64_t phy_cr3 = get_cr3();
  const uint64_t *pml4_ptr = (uint64_t *)convert_paging_phys_to_vir(phy_cr3);


  uint64_t pml4_val = pml4_ptr[indices.pml4_index];
  *out_pml4 = &pml4_ptr[indices.pml4_index];
  uint64_t phys_pdpt = parse_physical_address_from_page_table_entry(pml4_val);
  uint64_t *pdpt = convert_paging_phys_to_vir(phys_pdpt);


  uint64_t pdpt_val = pdpt[indices.pdpt_index];
  *out_pdpt = &pdpt[indices.pdpt_index];
  uint64_t phys_pd = parse_physical_address_from_page_table_entry(pdpt_val);
  uint64_t *pd = convert_paging_phys_to_vir(phys_pd);

  uint64_t pd_val = pd[indices.pd_index];
  *out_pd = &pd[indices.pd_index];
  uint64_t phys_pt = parse_physical_address_from_page_table_entry(pd_val);
  uint64_t *pt = convert_paging_phys_to_vir(phys_pt);

  uint64_t pt_val = pt[indices.pt_index];
  *out_pt = &pt[indices.pt_index];
}
uint64_t get_new_page_table_offset()
{
  return PAGE_TABLE_OFFSET;
}
inline struct vm_area get_area(enum area_type type)
{
  switch (type)
  {
    // kernel heap
    case 0:
      {
        return (struct vm_area){
          .start_address = 0xffffA00000000000ULL,
          .end_address = 0xffffB00000000000ULL,
        };
      }
    // paging area
    case 1:
      {
        return (struct vm_area){
          .start_address = 0xffffB00000000000ULL,
          .end_address = 0xffffC00000000000ULL,
        };
      }
    // allocation hader area
    case 2:
      {
        return (struct vm_area){
          .start_address = 0xffff050000000000ULL,
          .end_address = 0xffff100000000000ULL,
        };
      }
  }
}
void decode_page_table_params(uint64_t entry, page_table_params *p)
{
  p->present = (entry >> 0) & 1;
  p->rw = (entry >> 1) & 1;
  p->us = (entry >> 2) & 1;
  p->pwt = (entry >> 3) & 1;
  p->pcd = (entry >> 4) & 1;
  p->accessed = (entry >> 5) & 1;
  p->dirty = (entry >> 6) & 1;
  p->ps = (entry >> 7) & 1;
  p->global = (entry >> 8) & 1;

  p->avl = (entry >> 9) & 0x7;

  p->phys_addr = entry & 0x000FFFFFFFFFF000ULL;

  p->nx = (entry >> 63) & 1;
}
inline void *convert_paging_phys_to_vir(uint64_t phy)
{
  return (void *)((phy + PAGE_TABLE_OFFSET) | 0xffff000000000000ULL);
}
inline uint64_t convert_paging_vir_to_phys(void *vir)
{
  return ((uint64_t)vir - PAGE_TABLE_OFFSET);
}
int get_empty_slot_in_table(uint64_t *table, uint16_t starting_index, uint16_t ending_index, uint32_t length)
{
  uint32_t counter = 0;
  for (int i = starting_index; i <= ending_index; i++)
  {
    if (table[i] & 0x1)
    {
      counter = 0;
    }
    else
  {
      counter++;
      if (counter == length)
        return i;
    }
  }
  return -1;
}
uint64_t get_present_slot_in_table(uint64_t *table, uint16_t starting_index, uint16_t ending_index)
{
  for (int i = starting_index; i <= ending_index; i++)
  {
    if (table[i] & 0x1)
    {
      return table[i];
    }
  }
  return 0;
}
inline int get_present_entry_index(uint64_t *table){
  for(int i = 0;i < 512;i++){
    if ((table[i] & PTE_PRESENT))
      return i;
  }
  return -1;
}
uint32_t find_contiguous_empty_slots(uint64_t *table, uint64_t **out, void *prev, uint32_t prev_length, uint32_t length)
{
  uint32_t count = prev_length;
  uint64_t *start = prev;

  for (uint32_t i = 0; i < 512; i++)
  {
    if (table[i] & 0x1)
    {
      count = 0;
      start = NULL;
    }
    else
  {
      if (count == 0)
      {
        start = &table[i];
      }
      count++;

      if (count == length)
      {
        *out = start;
        return count;
      }
    }
  }

  *out = NULL;
  return 0;
}
void fill_empty_memory_slot(parsed_virtual_address indices, uint32_t length)
{

  uint64_t *pml4, *pdpt, *pd, *pt;
  parse_indices_to_pointer(indices, &pml4, &pdpt, &pd, &pt);
  printf("!!!\n");

  printf("pointers : 0x%p, 0x%p, 0x%p, 0x%p \n", pml4, pdpt, pd, pt);
  for (int i = 0; i < length; i++)
  {
    uint64_t physical_page = pmm_alloc_frame();

    //printf("physical address : 0x%p\n", physical_page);

    pt[i] = construct_pte((pte_params){
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
      .phys_addr = physical_page,
      .nx = 0});
  }
}



void *vmm_alloc(enum area_type type, uint32_t size)
{
  const struct vm_area area = get_area(type);

  parsed_virtual_address indices = find_virtual_memory_slot(&area, size);

  if(indices.pml4_index == 0&&indices.pdpt_index == 0&&indices.pd_index == 0&&indices.pt_index == 0)
    kernel_panic("OOM");

  printf("filling empty mem slot : size : 0x%p\n", size);
  fill_empty_memory_slot(indices, size);

  uint64_t address = build_virtual_address(indices);

  return (void *)address;
}
void vmm_free(struct vm_area *area, void *addr)
{
}


inline void clear_table(uint64_t* table){
  printf("table : 0x%p\n", table);
  for(int i = 0; i<512;i++){
      table[i] = 0;
  }
  printf("table : 0x%p\n", table);
}

void set_empty_pt_into_memory( parsed_virtual_address indices, uint64_t* pml4, uint64_t* pdpt, uint64_t *pd){
  uint64_t phys = pmm_alloc_frame();      // PT frame

  uint64_t* pt = load_physical_page_into_swap_page(phys);
  clear_table(pt);
  pt[indices.pt_index++] = make_table_pte(phys);   // present|rw|...
  unmap_swap();

  pd[indices.pd_index] = make_table_pte(phys);  // PDE -> PT
  uint64_t va = build_virtual_address(indices);
  /*
  uint64_t phys = pmm_alloc_frame();
  uint64_t pt_entry = pmm_alloc_frame();
 
  uint64_t *swap = load_physical_page_into_swap_page(phys);
  clear_table(swap);

  swap = load_physical_page_into_swap_page(pt_entry);
  clear_table(swap); 
  swap[0] = make_table_pte(phys);
  unmap_swap();

  printf("val in pd: 0x%p \n", pd[0x3F]);
  
  pd[indices.pd_index] = make_table_pte(phys);

  
  printf("val in pd: 0x%p \n", pd[indices.pd_index]);


  uint64_t table = build_virtual_address(indices);
  clear_table((uint64_t*)table);*/
}
void set_empty_pd_into_memory(const parsed_virtual_address indices, uint64_t* pml4, uint64_t* pdpt){
  uint64_t pd = pmm_alloc_frame();
  uint64_t pt = pmm_alloc_frame();


  uint64_t* swap= NULL;

  swap = load_physical_page_into_swap_page(pd);
  memset(swap, 0, 4096);
  swap[0] = make_table_pte(pt);
  

  swap = load_physical_page_into_swap_page(pt);
  memset(swap, 0, 4096);

  pdpt[indices.pdpt_index] = make_table_pte(pd);
}
void set_empty_pdpt_into_memory(const parsed_virtual_address indices, uint64_t* pml4){
  uint64_t pdpt = pmm_alloc_frame();
  uint64_t pd = pmm_alloc_frame();
  uint64_t pt = pmm_alloc_frame();


  uint64_t* swap= NULL;

  swap = load_physical_page_into_swap_page(pdpt);
  memset(swap, 0, 4096);
  swap[0] = make_table_pte(pd);
  
  swap = load_physical_page_into_swap_page(pd);
  memset(swap, 0, 4096);
  swap[0] = make_table_pte(pt);
  

  swap = load_physical_page_into_swap_page(pt);
  memset(swap, 0, 4096);

  pml4[indices.pml4_index] = make_table_pte(pdpt);
}
/*Use if vmm_alloc runs out of pages to map the allocated pages
 *
 * starting_index -> pml4 start of paging area
 * ending_index   -> pml4 end of paging area
 *
 * */
const parsed_virtual_address alloc_new_mapping_slot(uint64_t *pml4, uint16_t starting_index, uint16_t ending_index){
  printf("finding new paging slot\n");
  for(int i = starting_index ; i<= ending_index; i++){
    if (!(pml4[i] & PTE_PRESENT))
      continue;

    //found a present pml4 entry
    uint64_t* pdpt = pte_to_table_virt(pml4[i]);

    for(int k = 0; k<512;k++){
      uint64_t pdpt_e = pdpt[k];

      if(!(pdpt_e & PTE_PRESENT))
        continue;

      uint64_t* pd = pte_to_table_virt(pdpt_e);

      for(int j = 0; j<512;j++){
        uint64_t pd_e = pd[j];

        
        if((pd_e & PTE_PRESENT))
          continue;

        //val is present
        printf("case 0 0 0 x \n");
        /*case :
         * PML4   O
         * PDPT   O
         * PD     O
         * PT     X
         * */
      parsed_virtual_address indices = (parsed_virtual_address){
          .pml4_index = i,
          .pdpt_index = k,
          .pd_index = j,
          .pt_index = 0};

      printf("j : 0x%p \n", j);
      set_empty_pt_into_memory(indices, pml4, pdpt, pd);
      return indices;
      }     
      /*case :
         * PML4   O
         * PDPT   O
         * PD     X
         * PT     X
         * */
      parsed_virtual_address indices = (parsed_virtual_address){
          .pml4_index = i,
          .pdpt_index = k,
          .pd_index = 0,
          .pt_index = 0};

      set_empty_pd_into_memory(indices, pml4, pdpt);
      return indices;
    }
    /*case :
         * PML4   O
         * PDPT   X
         * PD     X
         * PT     X
         * */
    parsed_virtual_address indices = (parsed_virtual_address){
          .pml4_index = i,
          .pdpt_index = 0,
          .pd_index = 0,
          .pt_index = 0};

      set_empty_pdpt_into_memory(indices, pml4);

      return indices;
  }

  kernel_panic("OOM in pages ");

  parsed_virtual_address indices = (parsed_virtual_address){
          .pml4_index = 0,
          .pdpt_index = 0,
          .pd_index = 0,
          .pt_index = 0};

  return indices;

  /*case :
         * PML4   X
         * PDPT   X
         * PD     X
         * PT     X
         * */
}

