#pragma once

#include <stdint.h>

// from interrupt_wrapper.asm
#define ISR_STUB_SIZE 16

#define IDT_ENTRY_COUNT 256

#define KERNEL_CODE_SELECTOR 0x08

#define INTERRUPT_GATE_TYPE 0xE
#define TRAP_GATE_TYPE 0xF

#define PRIVILEGE_RING_3_USER 3
#define PRIVILEGE_RING_0_KERNEL 0

#define BUILD_GATE_DESCRIPTOR(entry, segment, ist, is_interrupt_gate, privilege_level) \
  ((union idt_entry){                                                                  \
      .data = {                                                                        \
          ((uint64_t)(entry) & 0xFFFFULL) |                                            \
              (((uint64_t)(segment) & 0xFFFFULL) << 16) |                              \
              (((uint64_t)(ist) & 0x7ULL) << 32) |                                     \
              ((uint64_t)((is_interrupt_gate) ? 0xE : 0xF) << 40) |                    \
              ((uint64_t)(privilege_level & 0x3) << 45) |                              \
              ((uint64_t)1 << 47) |                                                    \
              ((((uint64_t)(entry) >> 16) & 0xFFFFULL) << 48),                         \
                                                                                       \
          (((uint64_t)(entry) >> 32) & 0xFFFFFFFFULL)}})

typedef struct {
  uint16_t limit; // Größe der GDT - 1
  uint64_t base;  // Adresse der GDT
} __attribute__((packed)) idt_ptr_t;

// for decoding entries
struct InterruptDescriptor64 {
  uint16_t offset_1;       // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  uint8_t type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;       // offset bits 16..31
  uint32_t offset_3;       // offset bits 32..63
  uint32_t zero;           // reserved
};

union idt_entry {
  uint64_t data[2];
};

extern void global_interrupt_handler(uint16_t interrupt_num);

void init_idt(void);
