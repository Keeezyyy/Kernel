#include "idt.h"

#include "../../../debug/debug.h"
#include "../../../utils/utils.h"
#include <stdint.h>

union idt_entry {
  uint64_t data[2];
};

union idt_entry _build_gate_descriptor(uint64_t entry, uint16_t segment, uint8_t ist, bool is_interrupt_gate, uint8_t privilege_level) {

  union idt_entry curr_entry;

  curr_entry.data[0] = 0;
  curr_entry.data[1] = 0;

  curr_entry.data[0] |= (uint64_t)entry & 0xFFFF;
  curr_entry.data[0] |= ((uint64_t)segment << 16) & 0xFFFF0000;
  curr_entry.data[0] |= (uint64_t)(ist & 0x7) << 32;

  curr_entry.data[0] |= is_interrupt_gate ? (uint64_t)0xE << 40 : (uint64_t)0xF << 40;
  curr_entry.data[0] |= (uint64_t)(privilege_level & 0b11) << 45;
  curr_entry.data[0] |= (uint64_t)1 << 47;
  curr_entry.data[0] |= ((((uint64_t)entry) >> 16) & 0xFFFF) << 48;

  curr_entry.data[1] |= (((uint64_t)entry) >> 32) & 0xFFFFFFFF;

  return curr_entry;
}

void init_idt() {
  union idt_entry test = _build_gate_descriptor((uint64_t)init_idt, 0, 0, true, 3);
  debug_printf("val[0] : 0x%016llx\n", (uint64_t)init_idt);
  debug_printf("val[0] : 0x%016llx\n", test.data[0]);
  debug_printf("val[1] : 0x%016llx\n", test.data[1]);
}
