#pragma once
#include "../../hardware/hardware.h"
#include "../../utils/utils.h"
#include <stdint.h>

/* --- Register-Offsets (DLAB = 0) --- */
#define UART_RBR 0 /* R:  Receive Buffer Register        */
#define UART_THR 0 /* W:  Transmit Holding Register      */
#define UART_IER 1 /* RW: Interrupt Enable Register      */

/* --- Register-Offsets (DLAB = 1) --- */
#define UART_DLL 0 /* RW: Divisor Latch, Low Byte        */
#define UART_DLM 1 /* RW: Divisor Latch, High Byte       */

/* --- Register-Offsets (DLAB egal) --- */
#define UART_IIR 2 /* R:  Interrupt Identification Reg.  */
#define UART_FCR 2 /* W:  FIFO Control Register          */
#define UART_LCR 3 /* RW: Line Control Register (Bit 7 = DLAB) */
#define UART_MCR 4 /* RW: Modem Control Register         */
#define UART_LSR 5 /* R:  Line Status Register           */
#define UART_MSR 6 /* R:  Modem Status Register          */
#define UART_SCR 7 /* RW: Scratch Register               */

extern const uint16_t com_ports[8];

int init_seriel_port(uint32_t divisor, uint8_t port_num, bool parity_bit,
                     bool stop_bit);

void write_serial(char a, uint8_t port_num);

int is_transmit_empty(uint8_t port_num);

char read_serial(uint8_t port_num);

int serial_received(uint8_t port_num);
