#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#  define MAXNUM_CPU 8

#  define LENGTH_RAM (128 * 1024 * 1024)

// UART register in physical memory
#  define UART0 0x10000000L

// UART0 interrupt source
#  define UART0_IRQ 10

#define PLIC_BASE 0x0c000000L 
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4) // Priority of interrupt id
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4) // Pending status
#define PLIC_MENABLE(hart, id) (PLIC_BASE + 0x2000 + (hart) * 0x80 + ((id) / 32) * 4) // 为hart使能中断id
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)  // 优先级阈值
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000) // 等待读取中断ID
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000) // 完成中断


#endif /* __PLATFORM_H__ */