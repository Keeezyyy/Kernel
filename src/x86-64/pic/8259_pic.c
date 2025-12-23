#include "./8259_pic.h"


#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET  0x28

static inline uint8_t ps2_status(void) { return inb(0x64); }

static void ps2_flush_output(void) {
    while (ps2_status() & 1) {
        (void)inb(0x60);
    }
}

void init_pic_8259(){
  io_wait();
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  
  io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, PIC_MASTER_OFFSET);                 
	io_wait();
	outb(PIC2_DATA, PIC_SLAVE_OFFSET);                 
	io_wait();
	outb(PIC1_DATA, 1 << CASCADE_IRQ);        
	io_wait();
	outb(PIC2_DATA, 2);                       
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);               	
  io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

  outb(PIC1_DATA, 0xFF);
  outb(PIC2_DATA, 0xFF);
  pic_8259_clear_mask(0);
  pic_8259_clear_mask(1);

  ps2_flush_output();
}
void send_eoi_irq(uint8_t irq) {
    if (irq >= 8) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}

void pic_8259_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void pic_8259_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}
