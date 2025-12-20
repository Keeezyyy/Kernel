#include "./vmm.h"


uint64_t get_new_page_table_offset(){
  return PAGE_TABLE_OFFSET;
}

inline struct vm_area get_area(enum area_type type){
  switch (type) {
    //kernel heap
    case 0:
      {
        return (struct vm_area){
          .start_address = 0xffffA00000000000ULL,
          .end_address = 0xffffB00000000000ULL,
        };
      }
    //paging area
    case 1:
      {
        return (struct vm_area){
          .start_address = 0xffffB00000000000ULL,
          .end_address = 0xffffC00000000000ULL,
        };
      }
  }
}

void decode_page_table_params(uint64_t entry, page_table_params *p)
{
  p->present  = (entry >> 0) & 1;
  p->rw       = (entry >> 1) & 1;
  p->us       = (entry >> 2) & 1;
  p->pwt      = (entry >> 3) & 1;
  p->pcd      = (entry >> 4) & 1;
  p->accessed = (entry >> 5) & 1;
  p->dirty    = (entry >> 6) & 1;
  p->ps       = (entry >> 7) & 1;
  p->global   = (entry >> 8) & 1;

  p->avl      = (entry >> 9) & 0x7;

  p->phys_addr = entry & 0x000FFFFFFFFFF000ULL;

  p->nx       = (entry >> 63) & 1;
}

inline void* convert_paging_phys_to_vir(uint64_t phy){
  return (void*)((phy + PAGE_TABLE_OFFSET) | 0xffff000000000000ULL);
}

uint64_t get_empty_slot_in_table(uint64_t* table, uint16_t starting_index, uint16_t ending_index, uint32_t length){
  uint32_t counter = 0;
  for(int i = starting_index ; i<=ending_index; i++){
    if(table[i] &0x1){
      counter = 0;
    }else {
      counter++;
      if(counter == length)
        return table[i];
    }
  }
  return 0;
}
uint64_t get_present_slot_in_table(uint64_t* table, uint16_t starting_index, uint16_t ending_index){
  for(int i = starting_index ; i<=ending_index; i++){
    if(table[i] &0x1){
      return table[i];
    }
  }
  return 0;
}
uint32_t find_contiguous_empty_slots(uint64_t *table, uint64_t **out,void *prev, uint32_t prev_length,  uint32_t length){
  uint32_t count = prev_length;
  uint64_t *start = prev;

  for (uint32_t i = 0; i < 512; i++) {
    if (table[i] & 0x1) {
      count = 0;
      start = NULL;
    } else {
      if (count == 0) {
        start = &table[i];
      }
      count++;

      if (count == length) {
        *out = start;
        return count;
      }
    }
  }

  *out = NULL;
  return 0;
}

//TODO: search for overlapping contiguous pages too
const parsed_virtual_address get_small_page_slot(uint64_t* pml4,uint16_t starting_index,uint16_t ending_index,uint32_t length) {
    parsed_virtual_address result = {0};

    uint64_t *pt_start = NULL;
    uint32_t contiguous_pages = 0;

    for (uint16_t pml4_i = starting_index; pml4_i <= ending_index; pml4_i++) {

        if (!(pml4[pml4_i] & 0x1))
            continue;

        uint64_t phys_pdpt = parse_physical_address_from_page_table_entry(pml4[pml4_i]);
        uint64_t* pdpt = convert_paging_phys_to_vir(phys_pdpt);

        for (uint16_t pdpt_i = 0; pdpt_i < 512; pdpt_i++) {

            if (!(pdpt[pdpt_i] & 0x1))
                continue;

            uint64_t phys_pd = parse_physical_address_from_page_table_entry(pdpt[pdpt_i]);
            uint64_t* pd = convert_paging_phys_to_vir(phys_pd);

            for (uint16_t pd_i = 0; pd_i < 512; pd_i++) {

                if (!(pd[pd_i] & 0x1))
                    continue;

                uint64_t phys_pt = parse_physical_address_from_page_table_entry(pd[pd_i]);
                uint64_t* pt = convert_paging_phys_to_vir(phys_pt);

                contiguous_pages = find_contiguous_empty_slots(
                    pt,
                    &pt_start,
                    pt_start,
                    contiguous_pages,
                    length
                );

          
                if (contiguous_pages == length && pt_start) {
                    result.pml4_index = pml4_i;
                    result.pdpt_index = pdpt_i;
                    result.pd_index   = pd_i;
                    result.pt_index   = (uint16_t)(pt_start - pt);
                    return result;
                }
            }
        }
    }

    return result;
}

//TODO: use searhcing algorithm with better preformance
parsed_virtual_address find_virtual_memory_slot(struct vm_area *area, uint32_t page_length){
  const parsed_virtual_address starting_indices = parse_virtal_address(area->start_address);
  const parsed_virtual_address ending_indices = parse_virtal_address(area->end_address);

  printf("pml4_index : 0x%p, starting adr : 0x%p\n", starting_indices.pml4_index, area->start_address);
  printf("pml4_index : 0x%p, ending adr : 0x%p\n", ending_indices.pml4_index, area->end_address);

  const uint64_t phy_cr3 = get_cr3();
  const uint64_t* pml4_ptr = (uint64_t*)convert_paging_phys_to_vir(phy_cr3);

  uint64_t pml4_val = get_present_slot_in_table(pml4_ptr, starting_indices.pml4_index, ending_indices.pml4_index);
  if(pml4_val == 0)
    kernel_panic("no page table for vmm in pml4");

  const parsed_virtual_address slot_indices = get_small_page_slot(pml4_ptr,starting_indices.pml4_index, ending_indices.pml4_index,page_length);

  printf("pml4 : 0x%p, pdpt : 0x%p, pd : 0x%p, pt : 0x%p\n", slot_indices.pml4_index, slot_indices.pdpt_index, slot_indices.pd_index, slot_indices.pt_index);
  

  printf("pml4_val : 0x%p\n", pml4_val);
}



void* vmm_alloc(enum area_type type, uint32_t size){
  const struct vm_area area = get_area(type);

  find_virtual_memory_slot(&area, 5);
}

void  vmm_free(struct vm_area* area, void* addr){

}

uint64_t get_cr3(){
  uint64_t cr3 = 0;
  asm ("mov %%cr3, %0\n" :"=r"(cr3));
  return cr3;
}
