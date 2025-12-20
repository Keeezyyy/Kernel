#include "./mem_utils.h"

void decode_pte_utils(uint64_t entry, page_table_params *p)
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
 uint64_t make_pte_utils(page_table_params params)
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
