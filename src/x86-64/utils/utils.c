#include "./utils.h"

bool test_bit(uint8_t val, uint8_t index){
  return (val >> index) & 1;
}

