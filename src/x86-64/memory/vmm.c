#include "./vmm.h"

// UTILS
//------------------------------------------------------------------------------------------------------------------------------------------------------
static inline bool is_entry_present(uint64_t entry)
{
  return entry & PTE_PRESENT;
}
static inline uint64_t construct_pte(pte_params params)
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

  // ////printf("pte : 0x%p\n", entry);
  reload_cr3();
  return entry;
}
static inline bool cpu_supports_1gb_pages(void)
{
  uint32_t eax, ebx, ecx, edx;
  cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
  return (edx & (1u << 26)) != 0;
}
static inline uint64_t *get_pml4()
{
  return (uint64_t *)get_va_from_physical_address(get_cr3());
}
inline const struct vm_area get_area(enum virtual_mem_area_enum type)
{
  switch (type)
  {
  // kernel heap
  case KERNEL_HEAP:
  {
    return (struct vm_area){
        .start_address = START_KERNEL_HEAP,
        .end_address = END_KERNEL_HEAP,
    };
  }
  // free list
  case FREE_LIST:
  {
    return (struct vm_area){
        .start_address = START_FREE_LIST,
        .end_address = END_FREE_LIST,
    };
  }
  }
}
static inline bool is_table_empty(uint64_t *table){
  for(int i = 0; i< 512;i++){
    if(is_entry_present(table[i])){
      printf("[0x%p]entry : 0x%p\n", i, table[i]);
      return false;
    }
  }
  return true;
}
static inline bool test_bit(uint64_t val, uint8_t index)
{
  return ((val >> index) & 1);
}
static inline void clear_table(uint64_t *table)
{
  for (int i = 0; i < 512; i++)
  {
    table[i] = 0;
  }
}
static inline int get_empty_slot_in_table(uint64_t *table, uint16_t start_index, uint16_t end_index)
{
  for (int i = start_index; i <= end_index; i++)
  {
    if (!(table[i] & PTE_PRESENT))
      return i;
  }
  return -1;
}
static inline uint64_t get_physical_address_from_pte(uint64_t pte){
  return pte & 0x000FFFFFFFFFF000ULL;
}
static inline uint64_t *pte_to_table_virt(uint64_t entry)
{
  uint64_t adr = get_physical_address_from_pte(entry);
  return get_va_from_physical_address(adr);
}
static void parse_pointers_from_indices(parsed_virtual_address indices, uint64_t **out_pml4, uint64_t **out_pdpt, uint64_t **out_pd, uint64_t **out_pt)
{
  const uint64_t *pml4_ptr = get_pml4();

  uint64_t pml4_val = pml4_ptr[indices.pml4_index];
  *out_pml4 = &pml4_ptr[indices.pml4_index];
  uint64_t phys_pdpt = PTE_GET_ADDR(pml4_val);
  uint64_t *pdpt = get_va_from_physical_address(phys_pdpt);

  uint64_t pdpt_val = pdpt[indices.pdpt_index];
  *out_pdpt = &pdpt[indices.pdpt_index];
  uint64_t phys_pd = PTE_GET_ADDR(pdpt_val);
  uint64_t *pd = get_va_from_physical_address(phys_pd);

  uint64_t pd_val = pd[indices.pd_index];
  *out_pd = &pd[indices.pd_index];
  uint64_t phys_pt = PTE_GET_ADDR(pd_val);
  uint64_t *pt = get_va_from_physical_address(phys_pt);

  uint64_t pt_val = pt[indices.pt_index];
  *out_pt = &pt[indices.pt_index];
}
static inline bool is_entry_huge(uint64_t entry)
{
  return entry & PTE_PS;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

// Init
void vmm_init(){
  // memory.h util function to parse table indices from va
  const parsed_virtual_address indices = parse_virtal_address(vmm_get_new_page_table_offset());

  uint64_t *pml4_table = (uint64_t *)get_va_from_physical_address(get_cr3());
  uint64_t *pdpt = (uint64_t *)get_va_from_physical_address(pml4_table[indices.pml4_index] & 0x000FFFFFFFFFF000ULL);

  // TODO: implement direct mapping with 2mb pagse
  if (!cpu_supports_1gb_pages())
    kernel_panic("cpu does not support 1gb pages");

  for (int i = 0; i < MAX_RAM_IN_GB; i++)
  {

    pdpt[i] = PTE_GET_ADDR(i * GIGABYTE) |
              PTE_PRESENT |
              PTE_RW |
              PTE_PS;
  }

  reload_cr3();
}
// cr3 logic
//------------------------------------------------------------------------------------------
static inline uint64_t get_cr3()
{
  uint64_t cr3 = 0;
  asm("mov %%cr3, %0\n" : "=r"(cr3));
  return cr3;
}
static inline void reload_cr3()
{
  asm volatile(
      "mov %%cr3, %%rax\n"
      "mov %%rax, %%cr3\n"
      :
      :
      : "rax", "memory");
}
static inline void *get_va_from_physical_address(uint64_t physical_address)
{
  return (void *)DIRECT_MAPPING_OFFSET + physical_address;
}
//------------------------------------------------------------------------------------------
// gloabal function to return DIRECT_MAPPING_OFFSET
uint64_t vmm_get_new_page_table_offset()
{
  return DIRECT_MAPPING_OFFSET;
}
//------------------------------------------------------------------------------------------

// Alloc utils
//------------------------------------------------------------------------------------------
static inline uint64_t pte_construct(uint64_t adr)
{
  return PTE_GET_ADDR(adr) | PTE_PRESENT | PTE_RW;
}
static int find_contigouos_slot_in_table(uint64_t *table, uint16_t size, uint16_t start_index, uint16_t end_index)
{
  uint16_t count = 0;
  int ptr = -1;

  for (int i = start_index; i <= end_index; i++)
  {
    uint64_t entry = table[i];
    if (is_entry_present(entry))
    {
      count = 0;
      ptr = -1;
    }
    else
    {
      if (ptr == -1)
        ptr = i;
      count++;

      if (count == size)
        return ptr;
    }
  }
  return -1;
}
static const parsed_virtual_address find_contigouos_slot(const uint64_t const *pml4, const struct vm_area *area, uint32_t size)
{

  const parsed_virtual_address starting_indices = parse_virtal_address(area->start_address);
  const parsed_virtual_address ending_indices = parse_virtal_address(area->end_address);

  // TODO: support larger pages
  for (int i = starting_indices.pml4_index; i < ending_indices.pml4_index; i++)
  {
    uint64_t pml4_e = pml4[i];

    if (!is_entry_present(pml4_e))
      continue;

    uint64_t *pdpt = pte_to_table_virt(pml4_e);

    for (int k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];

      if (!is_entry_present(pdpt_e))
        continue;

      uint64_t *pd = pte_to_table_virt(pdpt_e);

      for (int j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        if (!is_entry_present(pd_e))
          continue;

        uint64_t *pt = pte_to_table_virt(pd_e);
        int slot = find_contigouos_slot_in_table(pt, size, 0, 512);

        if (slot < 0)
          continue;

        return (parsed_virtual_address){
            .pml4_index = i,
            .pdpt_index = k,
            .pd_index = j,
            .pt_index = slot,
        };
      }
    }
  }

  return (parsed_virtual_address){
      .pml4_index = 0,
      .pdpt_index = 0,
      .pd_index = 0,
      .pt_index = 0,
  };
}
static void set_empty_pdpt_into_memory(const parsed_virtual_address indices, uint64_t *pml4)
{
  // new pdpt, pd, pt
  // allocating frame, clearing and mapping page into page table
  // //printf("set empty pdpt_ into memory\n");
  uint64_t pdpt_physical_address = pmm_alloc_frame();
  uint64_t *new_pdpt = get_va_from_physical_address(pdpt_physical_address);
  clear_table(new_pdpt);
  pml4[indices.pml4_index] = pte_construct(pdpt_physical_address);

  uint64_t pd_physical_address = pmm_alloc_frame();
  uint64_t *new_pd = get_va_from_physical_address(pd_physical_address);
  clear_table(new_pd);
  new_pdpt[indices.pdpt_index] = pte_construct(pd_physical_address);

  uint64_t pt_physical_address = pmm_alloc_frame();
  uint64_t *new_pt = get_va_from_physical_address(pt_physical_address);
  clear_table(new_pt);
  new_pd[indices.pd_index] = pte_construct(pt_physical_address);
  reload_cr3();
}
static void set_empty_pd_into_memory(const parsed_virtual_address indices, uint64_t *pml4, uint64_t *pdpt)
{
  // //printf("set_empty_pd_into_memory\n");
}
static void set_empty_pt_into_memory(const parsed_virtual_address indices, uint64_t *pml4, uint64_t *pdpt, uint64_t *pd)
{
  // //printf("set_empty_pt_into_memory\n");
}
static const parsed_virtual_address alloc_new_mapping_slot(uint64_t *pml4, const struct vm_area *area, uint32_t size)
{
  // //printf("alloc new mapping slot\n");
  const parsed_virtual_address starting_indices = parse_virtal_address(area->start_address);
  const parsed_virtual_address ending_indices = parse_virtal_address(area->end_address);

  const uint16_t starting_index = starting_indices.pml4_index;
  const uint16_t ending_index = ending_indices.pml4_index;

  for (uint16_t i = starting_index; i <= ending_index; i++)
  {

    if (!is_entry_present(pml4[i]))
    {
      parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = 0, .pd_index = 0, .pt_index = 0};
      set_empty_pdpt_into_memory(idx, pml4);
      return idx;
    }

    uint64_t *pdpt = pte_to_table_virt(pml4[i]);

    for (uint16_t k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];



      // PDPT not present -> allocate PD here
      if (!is_entry_present(pdpt_e))
      {
        parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = k, .pd_index = 0, .pt_index = 0};
        set_empty_pd_into_memory(idx, pml4, pdpt);
        return idx;
      }

      uint64_t *pd = pte_to_table_virt(pdpt_e);

      for (uint16_t j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        // PD not present -> allocate PT here
        if (!is_entry_present(pd_e))
        {
          parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = k, .pd_index = j, .pt_index = 0};
          set_empty_pt_into_memory(idx, pml4, pdpt, pd);
          return idx;
        }
      }
    }
  }

  kernel_panic("OOM in paging area");
}
static void finilize_new_mapping(const parsed_virtual_address indices, uint32_t length)
{

  uint64_t *pml4, *pdpt, *pd, *pt;
  parse_pointers_from_indices(indices, &pml4, &pdpt, &pd, &pt);

  // //printf("pointers : 0x%p, 0x%p, 0x%p, 0x%p \n", pml4, pdpt, pd, pt);
  for (int i = 0; i < length; i++)
  {
    uint64_t physical_page = pmm_alloc_frame();
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

// Free Utils
static struct free_list_descriptor *get_empty_free_slot()
{
  //printf("x\n");
  constexpr uint16_t start_index = (START_FREE_LIST >> 39) & 0x1FF;
  constexpr uint16_t end_index = (END_FREE_LIST >> 39) & 0x1FF;

  //printf("x\n");
  uint64_t *pml4 = get_pml4();
  for (int i = start_index; i <= end_index; i++)
  {
    uint64_t pml4_e = pml4[i];

    if (!is_entry_present(pml4_e) && !is_entry_huge(pml4_e))
      continue;

    uint64_t *pdpt = pte_to_table_virt(pml4_e);

    for (int k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];

      if (!is_entry_present(pdpt_e) && !is_entry_huge(pdpt_e))
        continue;

      uint64_t *pd = pte_to_table_virt(pdpt_e);

      for (int j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        if (!is_entry_present(pd_e) && !is_entry_huge(pd_e))
          continue;

        uint64_t *pt = pte_to_table_virt(pd_e);

        for (int p = 0; p < 512; p++)
        {
          uint64_t pt_e = pt[p];

          if (!is_entry_present(pt_e) && !is_entry_huge(pt_e))
            continue;

          struct free_list_descriptor *page = (struct free_list_descriptor *)pte_to_table_virt(pt_e);
          //printf("page : 0x%p\n", page);

          for (int m = 0; m < 4096 / sizeof(struct free_list_descriptor); m++)
          {

            if (page[m].virtual_address == 0 && page[m].size == 0){
              struct free_list_descriptor* res = &page[m];
              return res;
            }
          }
        }
      }
    }
  }

  return (struct free_list_descriptor *)0x0;
}
static parsed_virtual_address get_free_list_slot(uint64_t *pml4){
  constexpr uint64_t size = 1;

  const parsed_virtual_address starting_indices = parse_virtal_address(START_FREE_LIST);
  const parsed_virtual_address ending_indices = parse_virtal_address(END_FREE_LIST);

  //printf("indices : 0x%p  -   0x%p\n", starting_indices.pml4_index, ending_indices.pml4_index);

  // TODO: support larger pages
  for (int i = starting_indices.pml4_index; i < ending_indices.pml4_index; i++)
  {
    uint64_t pml4_e = pml4[i];

    if (!is_entry_present(pml4_e))
      continue;

    uint64_t *pdpt = pte_to_table_virt(pml4_e);

    for (int k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];

      if (!is_entry_present(pdpt_e))
        continue;

      uint64_t *pd = pte_to_table_virt(pdpt_e);

      for (int j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        if (!is_entry_present(pd_e))
          continue;

        // //printf("pd_e : 0x%p\n", pd_e);
        uint64_t *pt = pte_to_table_virt(pd_e);
        int slot = find_contigouos_slot_in_table(pt, size, 0, 512);
        // //printf("slot : 0x%p\n", slot);
        if (slot < 0)
          continue;

        return (parsed_virtual_address){
            .pml4_index = i,
            .pdpt_index = k,
            .pd_index = j,
            .pt_index = slot,
        };
      }
    }
  }

  return (parsed_virtual_address){
      .pml4_index = 0,
      .pdpt_index = 0,
      .pd_index = 0,
      .pt_index = 0,
  };

}
static const parsed_virtual_address alloc_new_free_list_mapping_slot(uint64_t *pml4){
  const parsed_virtual_address starting_indices = parse_virtal_address(START_FREE_LIST);
  const parsed_virtual_address ending_indices = parse_virtal_address(END_FREE_LIST);


  const uint16_t starting_index = starting_indices.pml4_index;
  const uint16_t ending_index = ending_indices.pml4_index;

  for (uint16_t i = starting_index; i <= ending_index; i++)
  {

    if (!(pml4[i] & PTE_PRESENT))
    {
      parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = 0, .pd_index = 0, .pt_index = 0};
      set_empty_pdpt_into_memory(idx, pml4);
      return idx;
    }

    uint64_t *pdpt = pte_to_table_virt(pml4[i]);

    for (uint16_t k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];

      // PDPT not present -> allocate PD here
      if (!(pdpt_e & PTE_PRESENT))
      {
        parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = k, .pd_index = 0, .pt_index = 0};
        set_empty_pd_into_memory(idx, pml4, pdpt);
        return idx;
      }

      uint64_t *pd = pte_to_table_virt(pdpt_e);

      for (uint16_t j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        // PD not present -> allocate PT here
        if (!(pd_e & PTE_PRESENT))
        {
          parsed_virtual_address idx = {.pml4_index = i, .pdpt_index = k, .pd_index = j, .pt_index = 0};
          set_empty_pt_into_memory(idx, pml4, pdpt, pd);
          return idx;
        }
      }
    }
  }

  kernel_panic("OOM in paging area");
}
static void create_new_free_list_entry(uint64_t va, uint32_t size)
{
  //printf("test\n");
  struct free_list_descriptor *slot = get_empty_free_slot();

  //printf("sdfsf\n");
  if ((slot < START_FREE_LIST || slot > END_FREE_LIST) && slot != 0){
    //printf("slot : 0x%p\n", slot);
    //TODO: find better infavlid adr checKer
    //kernel_panic("something went wrong searching for free slot ");
  }

  //printf("test\n");
  if (slot == 0x0)
  {
    //Mallocing new page for slot
    {
      uint64_t *pml4 = get_pml4();
      parsed_virtual_address indices = get_free_list_slot(pml4);
      if (indices.pml4_index == 0 && indices.pdpt_index == 0 && indices.pd_index == 0 && indices.pt_index == 0)
      {
        indices = alloc_new_free_list_mapping_slot(pml4);

        finilize_new_mapping(indices, 1);

        slot = (struct free_list_descriptor *)build_virtual_address(indices);

        clear_table((void*)slot);

        if(!is_table_empty((uint64_t*)slot))
          kernel_panic("cleaing table error");
      }
    }
  }

  //TODO: fix this initual bits issue
  slot->virtual_address = va;
  slot->size = size;
  //printf("slot : 0x%p\n", slot);
}
static void free_physical_slots(uint32_t size, uint64_t *pml4, uint64_t *pdpt, uint64_t *pd, uint64_t *pt){
  printf("freeing : 0x%p - 0x%p\n", pt, pt+size);
  for(int i = 0; i<size;i++){
    pmm_free_frame(pt[i] & 0x000FFFFFFFFFF000ULL);
    pt[i] = 0;
  }
}
static void free_paging_tables(uint64_t *pml4, uint64_t *pdpt, uint64_t *pd, uint64_t *pt){

  printf("freeing paging tables\n");

  //test if pt is empty
  if(is_table_empty(pte_to_table_virt(*pd))){
    pmm_free_frame(get_physical_address_from_pte(*pd));
    *pd = 0;
    printf("pt id empty\n");
  }

  //test if pd is empty
  if(is_table_empty(pte_to_table_virt(*pdpt))){
    pmm_free_frame(get_physical_address_from_pte(*pdpt));
    *pdpt = 0;
    printf("pd id empty\n");
  }

  //test if pdpt is empty
  if(is_table_empty(pte_to_table_virt(*pml4))){
    pmm_free_frame(get_physical_address_from_pte(*pml4));
    *pml4 = 0;
    printf("pdpt id empty\n");
  }
}
static void free_slot(uint64_t va, uint32_t size){
  printf("freeing slot : 0x%p, for 0x%p pages \n", va, size);

  const parsed_virtual_address indices = parse_virtal_address(va);

  printf("indieces : 0x%p, 0x%p, 0x%p, 0x%p\n", indices.pml4_index, indices.pdpt_index, indices.pd_index, indices.pt_index);

  uint64_t *pml4, *pdpt, *pd, *pt;

  parse_pointers_from_indices(indices, &pml4, &pdpt, &pd, &pt);

  free_physical_slots(size, pml4, pdpt, pd, pt);

  free_paging_tables(pml4, pdpt, pd, pt);
}
//------------------------------------------------------------------------------------------

// Free
void vmm_free(void *va)
{
  constexpr uint16_t start_index = (START_FREE_LIST >> 39) & 0x1FF;
  constexpr uint16_t end_index = (END_FREE_LIST >> 39) & 0x1FF;

  printf("freeing : 0x%p\n", va);
  uint64_t *pml4 = get_pml4();
  //printf("pml4 : 0x%p\n", pml4);
  for (int i = start_index; i <= end_index; i++)
  {
    uint64_t pml4_e = pml4[i];

    if (!is_entry_present(pml4_e) && !is_entry_huge(pml4_e))
      continue;

    uint64_t *pdpt = pte_to_table_virt(pml4_e);

    //printf("pml4_e : 0x%p\n", pml4_e);
    //printf("pdpt : 0x%p\n", pdpt);
    for (int k = 0; k < 512; k++)
    {
      uint64_t pdpt_e = pdpt[k];

      if (!is_entry_present(pdpt_e) && !is_entry_huge(pdpt_e))
        continue;

      uint64_t *pd = pte_to_table_virt(pdpt_e);
      //printf("pdpt_e : 0x%p\n", pdpt_e);

      //printf("pd : 0x%p\n", pd);
      for (int j = 0; j < 512; j++)
      {
        uint64_t pd_e = pd[j];

        if (!is_entry_present(pd_e) && !is_entry_huge(pd_e))
          continue;

        uint64_t *pt = pte_to_table_virt(pd_e);

        //printf("pt : 0x%p\n", pt);
        for (int p = 0; p < 512; p++)
        {
          uint64_t pt_e = pt[p];

          if (!is_entry_present(pt_e) && !is_entry_huge(pt_e))
            continue;

          struct free_list_descriptor *page = (struct free_list_descriptor *)pte_to_table_virt(pt_e);
          //printf("page : 0x%p\n", page);

          for (int m = 0; m < 4096 / sizeof(struct free_list_descriptor); m++)
          {

            if (page[m].virtual_address == (uint64_t)va && page[m].size != 0){
              free_slot((uint64_t)va, page[m].size);
              printf("freeing 0x%p for 0x%p bytes \n", va, page[m].size);
              return;
            }

          }
        }
      }
    }
  }

  kernel_panic("free discriptor not found");

}

// Allocation
void *vmm_alloc(uint32_t size, enum virtual_mem_area_enum type)
{
  uint64_t *pml4 = get_pml4();

  struct vm_area area = get_area(type);

  // //printf("\n\n");
  parsed_virtual_address indices = find_contigouos_slot(pml4, &area, size);
  if (indices.pml4_index == 0 && indices.pdpt_index == 0 && indices.pd_index == 0 && indices.pt_index == 0)
  {
    // no empty slot ->
    // have to create new
    // TODO: support bigger allocations
    // //printf("\n indieces : pml4 : 0x%p, pdpt : 0x%p, pd : 0x%p, pt : 0x%p\n", indices.pml4_index, indices.pdpt_index, indices.pd_index, indices.pt_index);
    if (size < 512)
      indices = alloc_new_mapping_slot(pml4, &area, size);
    if (size >= 512)
      kernel_panic("doesnt support large allocations");
  }

  printf("indieces : pml4 : 0x%p, pdpt : 0x%p, pd : 0x%p, pt : 0x%p\n", indices.pml4_index, indices.pdpt_index, indices.pd_index, indices.pt_index);

  finilize_new_mapping(indices, size);

  uint64_t va = build_virtual_address(indices);

  create_new_free_list_entry(va, size);

  return (void *)va;
}
