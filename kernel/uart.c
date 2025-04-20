#include "include/os.h"

#define UART_REG(reg) ((volatile uint8_t*)(UART0 + reg))

#define RHR 0  // Receive Holding Register (read mode)
#define THR 0  // Transmit Holding Register (write mode)
#define DLL 0  // LSB of Divisor Latch (write mode)
#define IER 1  // Interrupt Enable Register (write mode)
#define DLM 1  // MSB of Divisor Latch (write mode)
#define FCR 2  // FIFO Control Register (write mode)
#define ISR 2  // Interrupt Status Register (read mode)
#define LCR 3  // Line Control Register
#define MCR 4  // Modem Control Register
#define LSR 5  // Line Status Register
#define MSR 6  // Modem Status Register
#define SPR 7  // ScratchPad Register

#define LSR_RX_READY (1 << 0)  // receive data ready
#define LSR_TX_IDLE (1 << 5)   // transmit holding register empty

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, value) (*(UART_REG(reg)) = (value))

void uart_init() {
  // 关中断
  uart_write_reg(IER, 0x00);

  // 设置波特率
  uint8_t lcr = uart_read_reg(LCR);
  uart_write_reg(LCR, lcr | (1 << 7));
  uart_write_reg(DLL, 0x03);  // 115200 baud rate
  uart_write_reg(DLM, 0x00);  // 115200 baud rate

  // 8 data bits, 1 stop bit, no parity
  lcr = 0;
  uart_write_reg(LCR, lcr | (3 << 0));
}

int uart_putc(int c) {
  while (0 == (uart_read_reg(LSR) & LSR_TX_IDLE)) {
    ;
  }
  return uart_write_reg(THR, c);
}

void uart_puts(const char* s) {
  while (*s) {
    uart_putc(*s++);
  }
}
void uart_getc(char* c) {
  while (0 == (uart_read_reg(LSR) & LSR_RX_READY)) {
    ;
  }
  *c = uart_read_reg(RHR);
}
