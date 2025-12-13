#include "./stdio.h"



void memcpy(void * dst, const void * src, uint32_t n){
  char *dst_tmp = (char*)dst;
  char *src_tmp = (char*)src;

  for(int i = 0; i<n;i++){
    *dst_tmp++ = *src_tmp++;
  }
}
