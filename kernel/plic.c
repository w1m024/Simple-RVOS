#include "os.h"

void plic_init(void) {
  int hart = r_tp();

  *(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = 1;

  *(uint32_t*)PLIC_MENABLE(hart, UART0_IRQ) = 1 << (UART0_IRQ % 32);

  *(uint32_t*)PLIC_MTHRESHOLD(hart) = 0;

  // 启动软件中断(MSIE)， 定时器中断(MTIE), 外部中断(MEIE)
  w_mie(r_mie() | MIE_MSIE | MIE_MTIE | MIE_MEIE);

  w_mstatus(r_mstatus() | MSTATUS_MIE);
}

int plic_claim(void) {
  int hart = r_tp();
  int irq = *(uint32_t*)PLIC_MCLAIM(hart);
  return irq;
}

void plic_complete(int irq) {
  int hart = r_tp();
  *(uint32_t*)PLIC_MCOMPLETE(hart) = irq;
}
