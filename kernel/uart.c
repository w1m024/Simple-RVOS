#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

#define UART0 0x10000000L	// Base address for COM1 (UART0)

#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE (1 << 5)

#define UART_REG(reg) ((volatile unsigned char *)(UART0 + (reg)))

#define uart_write_reg(reg, value) \
    (*(volatile unsigned char *)(UART_REG(reg)) = (value))
#define uart_read_reg(reg) \
    (*(volatile unsigned char *)(UART_REG(reg)))

void uart_init(void) {
    uart_write_reg(IER, 0x00); // Disable all interrupts

    uart_write_reg(LCR, uart_read_reg(LCR) | (1 << 7));
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);

    uart_write_reg(LCR, 0x03); // 8 bits, no parity, 1 stop bit
    
}

void uart_putc(char ch) {
    while (!(uart_read_reg(LSR) & LSR_TX_IDLE)); // Wait for THR to be empty
    uart_write_reg(THR, ch);
}

void uart_puts(char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}