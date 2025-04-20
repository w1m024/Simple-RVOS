extern void uart_init(void);
extern int uart_putc(int c);
extern void uart_puts(const char* s);

void start_kernel(void) {

  uart_init();
  uart_putc('H');
  uart_puts("Hello, world!\n");

  while (1) {};
}
