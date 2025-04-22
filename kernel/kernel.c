
extern void uart_init(void);
extern void uart_puts(char* str);

void start_kernel(void) {
  uart_init();
  uart_puts("Hello, world!\n");

  while (1) {};
}