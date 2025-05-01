#include "os.h"

/* defined in entry.S */
extern void trap_vector(void);

extern void uart_isr(void);
extern void timer_handler(void);

void trap_init(void) {
  uart_puts("trap_init\n");
  w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler() {
  int irq = plic_claim();

  if (irq == UART0_IRQ) {
    uart_isr();
  } else if (irq) {
    printf("unexpected interrupt irq = %d\n", irq);
  }

  if (irq) { plic_complete(irq); }
}

reg_t trap_handler(reg_t epc, reg_t cause) {

  reg_t return_pc = epc;
  reg_t cause_code = cause & MCAUSE_MASK_ECODE;

  if (cause & MCAUSE_MASK_INTERRUPT) {
    // interrupt
    switch (cause_code) {
    case 3:
      uart_puts("software interruption!\n");

      // 表示中断已处理
      int id = r_mhartid();
      *(uint32_t*)CLINT_MSIP(id) = 0;

      schedule();

      break;
    case 7:
      uart_puts("timer interruption!\n");
      timer_handler();
      break;
    case 11:
      uart_puts("external interruption!\n");
      external_interrupt_handler();
      break;
    default: printf("Unknown async exception! Code = %ld\n", cause_code); break;
    }
  } else {
    // exception
    printf("Sync exceptions! Code = %ld\n", cause_code);
    panic("OOPS! What can I do!");
    // return_pc += 4;
  }

  return return_pc;
}

void trap_test(void) {
  /*
   * Synchronous exception code = 7
   * Store/AMO access fault
   */
  *(int*)0x00000000 = 100;

  /*
   * Synchronous exception code = 5
   * Load access fault
   */
  // int a = *(int*)0x00000000;

  uart_puts("back from trap.\n");
}