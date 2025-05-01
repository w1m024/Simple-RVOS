#include "os.h"

extern void schedule(void);

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;

#define MAX_TIMER 10
static struct timer timer_list[ MAX_TIMER ];

void timer_load(int interval) {
  int id = r_mhartid();

  *(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init(void) {
  uart_puts("timer_init\n");
  struct timer* t = &(timer_list[ 0 ]);
  for (int i = 0; i < MAX_TIMER; i++) {
    t->func = NULL;
    t->arg = NULL;
    t++;
  }

  timer_load(TIMER_INTERVAL);
}

struct timer* timer_create(void (*handler)(void* arg), void* arg, uint32_t timeout_tick) {
  if (NULL == handler || 0 == timeout_tick) return NULL;

  spin_lock();

  struct timer* t = &(timer_list[ 0 ]);
  int i;
  for (i = 0; i < MAX_TIMER; i++) {
    if (NULL == t->func) { break; }
    t++;
  }

  if (i >= MAX_TIMER) {
    spin_unlock();
    return NULL;
  }

  t->func = handler;
  t->arg = arg;
  t->timeout_tick = _tick + timeout_tick;

  spin_unlock();

  return t;
}

void timer_delete(struct timer* timer) {
  spin_lock();
  struct timer* t = &(timer_list[ 0 ]);
  for (int i = 0; i < MAX_TIMER; i++) {
    if (t == timer) {
      t->func = NULL;
      t->arg = NULL;
      break;
    }
    t++;
  }

  spin_unlock();
}

static inline void timer_check() {
  struct timer* t = &(timer_list[ 0 ]);

  for (int i = 0; i < MAX_TIMER; i++) {
    if (NULL == t->func) {
      t++;
      continue;
    }

    if (t->timeout_tick <= _tick) {
      t->func(t->arg);
      t->func = NULL;
      t->arg = NULL;
    }
    t++;
  }
}

void timer_handler(void) {
  _tick++;

  printf("tick %d\n", _tick);

  timer_check();

  timer_load(TIMER_INTERVAL);

  schedule();
}