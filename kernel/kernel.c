#include "os.h"

extern void uart_init(void);
extern void uart_puts(char* str);
extern void page_init(void);
extern void page_test(void);

void start_kernel(void) {
  uart_init();
  uart_puts("Hello, world!\n");

  page_init();

  page_test();

  uart_puts("page test end.\n");
  while (1) {};
}