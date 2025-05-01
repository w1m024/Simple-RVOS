#ifndef __OS_H__
#define __OS_H__

#include "platform.h"
#include "riscv.h"
#include "types.h"

#include <stdarg.h>
#include <stddef.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char* s);

/* printf */
extern int printf(const char* s, ...);
extern void panic(char* s);

/* memory management */
extern void* page_alloc(int npages);
extern void page_free(void* p);

/* task management */
struct context {
  /* ignore x0 */
  reg_t ra;
  reg_t sp;
  reg_t gp;
  reg_t tp;
  reg_t t0;
  reg_t t1;
  reg_t t2;
  reg_t s0;
  reg_t s1;
  reg_t a0;
  reg_t a1;
  reg_t a2;
  reg_t a3;
  reg_t a4;
  reg_t a5;
  reg_t a6;
  reg_t a7;
  reg_t s2;
  reg_t s3;
  reg_t s4;
  reg_t s5;
  reg_t s6;
  reg_t s7;
  reg_t s8;
  reg_t s9;
  reg_t s10;
  reg_t s11;
  reg_t t3;
  reg_t t4;
  reg_t t5;
  reg_t t6;

  reg_t pc; //保存任务被中断时的mepc值，用于回复
};

extern int task_create(void (*task)(void));
extern void task_delay(volatile int count);
extern void task_yield(void);

// plic
extern void plic_complete(int irq);
extern int plic_claim(void);

// sched.c
void schedule(void);


// lock
extern int spin_lock(void);
extern int spin_unlock(void);

// software timer
struct timer {
  void (*func)(void* arg);  // 指向回调函数
  void* arg; // 回调函数参数
  uint32_t timeout_tick; // 超时时间
};
extern struct timer *timer_create(void (*handler)(void* arg), void* arg, uint32_t timeout_tick);
extern void timer_delete(struct timer* timer);
#endif /* __OS_H__ */
