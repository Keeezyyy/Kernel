#include "./stdio.h"



void memcpy(void * dst, const void * src, uint32_t n){
  char *dst_tmp = (char*)dst;
  char *src_tmp = (char*)src;

  for(int i = 0; i<n;i++){
    *dst_tmp++ = *src_tmp++;
  }
}
void *memset(void *d, int c, uint64_t len) {
  uint8_t *dest = (uint8_t*)d;
  uint8_t value = (uint8_t)c;

  for (uint64_t i = 0; i < len; i++) {
    dest[i] = value;
  }

  return d;
}
