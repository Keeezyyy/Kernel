#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"
#include "./isr_routines.h"

#define KERNEL_CODE_SELECTOR 0x08

#define INTERRUPT_GATE_TYPE 0xE
#define TRAP_GATE_TYPE 0xF

#define PRIVILEGE_RING_3_USER 3
#define PRIVILEGE_RING_0_KERNEL 0

typedef struct
{
  uint16_t offset_1;
  uint16_t selector;
  uint8_t ist;
  uint8_t type_attributes;
  uint16_t offset_2;
  uint32_t offset_3;
  uint32_t zero;
} __attribute__((packed)) InterruptDescriptor64;

typedef struct
{
  uint16_t size;
  uint64_t offset;
} __attribute__((packed)) IDTR;

void idt_set_entry_ex(InterruptDescriptor64 *entry,
                      uint64_t handler,
                      uint8_t ist,
                      uint8_t present,
                      uint8_t dpl,
                      uint8_t gate_type,
                      uint16_t selector);

void __attribute__((nooptimize))  init_idt();

void interrupt_handler(uint64_t isr_num);

void fill_idts();

extern void __attribute__((nooptimize)) load_idtr(void *);
extern void isr_wrapper();
