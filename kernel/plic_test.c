#include "os.h"

// 假设 PLIC 相关寄存器和中断号已定义
#define PLIC_ENABLE_OFFSET 0x2000

// 定义定时器寄存器地址（如 QEMU virt 平台）
#define MTIME 0x200BFF8
#define MTIMECMP 0x2004000

// 定义 MSIP 寄存器地址（用于触发软件中断）
#define MSIP_BASE 0x2000000

// 测试标志位
volatile int software_irq_triggered = 0;
volatile int timer_irq_triggered = 0;
volatile int external_irq_triggered = 0;

// 触发软件中断
void trigger_software_interrupt() {
  uart_puts("=== Testing Software Interrupt (case 3) ===\n");
  // 写 MSIP 寄存器触发软件中断
  *((volatile uint32_t*)MSIP_BASE) = 1;
}

// 触发定时器中断
void trigger_timer_interrupt() {
  uart_puts("=== Testing Timer Interrupt (case 7) ===\n");
  // 设置定时器比较寄存器为当前时间 + 1，立即触发中断
  uint64_t now = *((volatile uint64_t*)MTIME);
  *((volatile uint64_t*)MTIMECMP) = now + 1;
}

// 触发外部中断（模拟 UART 或 GPIO 中断）
void trigger_external_interrupt(int irq) {
  uart_puts("=== Testing External Interrupt (case 11) ===\n");
  // 配置 PLIC 并触发外部中断
  volatile uint32_t* plic_enable = (uint32_t*)(PLIC_BASE + PLIC_ENABLE_OFFSET);
  // 使能指定中断
  plic_enable[ irq / 32 ] |= (1 << (irq % 32));
  // 模拟外部中断（向 PLIC 发送中断请求）
  volatile uint32_t* plic_pending = (uint32_t*)(PLIC_BASE + 0x1000);
  plic_pending[ irq / 32 ] |= (1 << (irq % 32));
}

// 主测试函数
void plic_test() {

  // 1. 测试软件中断（case 3）
  trigger_software_interrupt();
  // 等待中断处理完成（实际场景可能需要忙等或触发后直接检查标志）
  while (!software_irq_triggered) {
    asm volatile("wfi");  // 等待中断
  }

  // 2. 测试定时器中断（case 7）
  trigger_timer_interrupt();
  while (!timer_irq_triggered) {
    asm volatile("wfi");
  }

  // 3. 测试外部中断（case 11）
  trigger_external_interrupt(UART0_IRQ);  // 测试 UART 中断
  while (!external_irq_triggered) {
    asm volatile("wfi");
  }

  // // 可选：测试其他外部中断（如 GPIO）
  // external_irq_triggered = 0;
  // trigger_external_interrupt(GPIO_IRQ);
  // while (!external_irq_triggered) {
  //     asm volatile ("wfi");
  // }

  uart_puts("=== All PLIC tests passed! ===\n");
}