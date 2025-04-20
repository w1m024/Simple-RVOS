extern void uart_init(void);
extern int uart_putc(int c);
extern void uart_puts(const char* s);
extern void uart_getc(char* c);

void start_kernel(void) {

  uart_init();
  uart_putc('H');
  uart_putc('\n');
  uart_puts("Hello, world!\n");

  while (1) {
    char c;
    uart_puts("Please input a character: ");
    uart_getc(&c);
    uart_putc(c);
    uart_puts("\n");
  };
  
}
