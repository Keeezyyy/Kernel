#include "./malloc.h"
#include "../../x86-64/stdio/stdio.h"
#include "./vmm.h"
#include <stdint.h>

// TODO:
/*
 *:)
 * quick impl.
 * fix later
 *
 *
 */
int k_malloc(uint32_t size, void **out) {

  int err = 0;

  uint32_t ceiled_size = CEIL_DIV(size, 4096);
  void *res = vmm_alloc(ceiled_size, KERNEL_HEAP, &err);

  if (err != 0)
    return -1;

  *out = res;
  return 1;
}

int k_free(void *ptr) {
  if (!vmm_free(ptr))
    return -1;

  return 1;
}
