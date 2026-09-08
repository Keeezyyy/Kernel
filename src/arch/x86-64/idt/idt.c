#include "idt.h"

#include "../../../debug/debug.h"
#include <stdint.h>

extern uint8_t isr_stub_table[256][ISR_STUB_SIZE];

static union idt_entry idt_buffer[IDT_ENTRY_COUNT];
static idt_ptr_t idtr;

static void _init_idt_entries(void) {
  // union idt_entry tmp = BUILD_GATE_DESCRIPTOR(&isr_wrapper, KERNEL_CODE_SELECTOR, 0, INTERRUPT_GATE_TYPE, PRIVILEGE_RING_0_KERNEL);

  for (int i = 0; i < 256; i++) {
    // for now
    // TODO: correct type and priv
    idt_buffer[i] = BUILD_GATE_DESCRIPTOR(&isr_stub_table[i], KERNEL_CODE_SELECTOR, 0, TRAP_GATE_TYPE, KERNEL_CODE_SELECTOR);
  }
}

void global_interrupt_handler(uint16_t interrupt_num) {
  debug_printf("[interrupt] : 0x%x\n", interrupt_num);

  panic("interrupt");
}

void init_idt(void) {
  _init_idt_entries();

  idtr.base = (uint64_t)idt_buffer;
  idtr.limit = sizeof(idt_buffer) - 1;

  __asm__("lidt %0" ::"m"(idtr));

  debug_printf("[IDT] : IDT Setup Complete\n");
}
