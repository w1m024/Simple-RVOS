#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context* next);

#define MAX_TASK_NUM 10
#define STACK_SIZE 1024

uint8_t __attribute__((aligned(16))) task_stack[ MAX_TASK_NUM ][ STACK_SIZE ];
struct context ctx_task[ MAX_TASK_NUM ];

static int _top = 0;
static int _current = -1;

// static void w_mscratch(reg_t x) { asm volatile("csrw mscratch, %0" : : "r"(x)); }

void sched_init() {
  uart_puts("sched_init\n");

  w_mscratch(0);
  w_mie(r_mie() | MIE_MSIE);
}

void schedule() {
  // struct context* next = &ctx_task;
  if (_top <= 0) {
    uart_puts("No task to schedule!\n");
    return;
  }
  _current = (_current + 1) % _top;
  struct context* next = &ctx_task[ _current ];
  switch_to(next);
}

int task_create(void (*start_routin)(void)) {
  if (_top >= MAX_TASK_NUM) {
    uart_puts("Task limit reached!\n");
    return -1;
  }

  ctx_task[ _top ].sp = (reg_t)&task_stack[ _top ][ STACK_SIZE ];
  ctx_task[ _top ].pc = (reg_t)start_routin;
  _top++;
  return 0;
}

void task_yield() {
  int id = r_mhartid();
  *(uint32_t*)CLINT_MSIP(id) = 1;
}

void task_delay(volatile int count) {
  count *= 100000;
  while (count--)
    ;
}
