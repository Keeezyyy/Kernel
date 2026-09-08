#pragma once

#include <stdint.h>

typedef struct {
  uint16_t offset_1;       // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t zero;            // unused, set to 0
  uint8_t type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed)) InterruptDescriptor32;

typedef struct {
  uint16_t limit; // Größe der GDT - 1
  uint64_t base;  // Adresse der GDT
} __attribute__((packed)) idt_ptr_t;

void init_idt();
