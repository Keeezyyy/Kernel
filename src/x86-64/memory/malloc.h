#pragma once
#include "../../utils/utils.h"
#include <stdint.h>

int k_malloc(uint32_t size, void **out);

int k_free(void *ptr);
