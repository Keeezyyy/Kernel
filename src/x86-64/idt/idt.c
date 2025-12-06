#include "./idt.h"

InterruptDescriptor64 idt_entries[256];
IDTR idtr_ptr;

void fill_idts(){
    idt_set_entry_ex(&idt_entries[0],(uint64_t)isr_wrapper, 0, 1, 0, 0xE, KERNEL_CODE_SELECTOR);
  
}

void idt_set_entry_ex(InterruptDescriptor64* entry,
                      uint64_t handler,
                      uint8_t ist,
                      uint8_t present,
                      uint8_t dpl,
                      uint8_t gate_type,
                      uint16_t selector)
{
    entry->offset_1 = handler & 0xFFFF;
    entry->selector = selector;

    entry->ist = ist & 0x07;

    
    uint8_t type_attr = 0;
    type_attr |= (present & 0x01) << 7;     
    type_attr |= (dpl & 0x03) << 5;         
    type_attr |= (gate_type & 0x0F);        

    entry->type_attributes = type_attr;

    entry->offset_2 = (handler >> 16) & 0xFFFF;
    entry->offset_3 = (handler >> 32) & 0xFFFFFFFF;

    entry->zero = 0;
}

void init_idt(){
    //TODO: implement the IST for the idt entries
    fill_idts();



    idtr_ptr.offset = (uint64_t)&idt_entries[0];
    idtr_ptr.size = sizeof(idt_entries)-1;

    load_idtr((void*)&idtr_ptr);
}

void interrupt_handler(){
    printf("iinterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\ninterrupt!\nnterrupt!\n");
  for(;;){}
}
