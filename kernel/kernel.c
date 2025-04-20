extern void uart_init(void);
extern int uart_getc(void);
extern void uart_putc(int c);
extern void uart_puts(const char* s);

void start_kernel(void) {

  uart_init();
  uart_puts("Hello, world!\n");
  uart_puts("This is a simple kernel.\n");

  while (1) {};
}
