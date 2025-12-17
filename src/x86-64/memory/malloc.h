#pragma once 
#include <stdint.h>


void *k_malloc(void *prefered_addresse,uint32_t size);

void k_free(void *ptr);
