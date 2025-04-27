#include "os.h"

extern void uart_init(void);
extern void uart_puts(char* str);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);

void start_kernel(void) {
  uart_init();
  uart_puts("Hello, world!\n");

  page_init();

  trap_init();

  sched_init();

  os_main();
  
  schedule();

  uart_puts("sched test end.\n");

  while (1) {};
}