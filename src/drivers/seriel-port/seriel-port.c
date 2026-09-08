#include "seriel-port.h"

const uint16_t com_ports[8] = {
    0x3F8, /* COM1 */
    0x2F8, /* COM2 */
    0x3E8, /* COM3 */
    0x2E8, /* COM4 */
    0x5F8, /* COM5 */
    0x4F8, /* COM6 */
    0x5E8, /* COM7 */
    0x4E8  /* COM8 */
};

// TODO:
static uint8_t configure_stop_parity(bool parity_bit, bool stop_bit) {

  return 0x03;
}

int init_seriel_port(uint32_t divisor, uint8_t port_num, bool parity_bit,
                     bool stop_bit) {

  uint16_t port = com_ports[port_num];
  outb(port + 1, 0x00);        // Disable all interrupts
  outb(port + UART_LCR, 0x80); // start the divisor configuaration

  // send divisor
  // baud = 115200 / divisor
  outb(port, divisor & 0xFF);
  outb(port + 1, (divisor >> 8) & 0xFF);
  //
  //

  outb(port + UART_LCR,
       configure_stop_parity(parity_bit,
                             stop_bit)); // clear the divisor bit and set stop
                                         // and parity configuaration

  outb(port + UART_FCR,
       0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(port + UART_MCR, 0x0B); // IRQs enabled, RTS/DSR set
  outb(port + UART_MCR, 0x1E); // Set in loopback mode, test the serial chip
  outb(port, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                    // returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (inb(port) != 0xAE) {
    return 1;
  }

  outb(port + UART_MCR, 0x0F);
  return 0;
}

int serial_received(uint8_t port_num) {
  return inb(com_ports[port_num] + 5) & 1;
}

char read_serial(uint8_t port_num) {
  while (serial_received(port_num) == 0)
    ;

  return inb(com_ports[port_num]);
}
int is_transmit_empty(uint8_t port_num) {
  return inb(com_ports[port_num] + 5) & 0x20;
}

void write_serial(char a, uint8_t port_num) {
  while (is_transmit_empty(port_num) == 0)
    ;

  outb(com_ports[port_num], a);
}
