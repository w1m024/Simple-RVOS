#include "os.h"

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;

void timer_load(int interval) {
  int id = r_mhartid();

  *(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init(void) {
  timer_load(TIMER_INTERVAL);

  w_mie(r_mie() | MIE_MTIE);             // Enable timer interrupt
  w_mstatus(r_mstatus() | MSTATUS_MIE);  // Enable global interrupt
}

void timer_handler(void) {
  _tick++;
  printf("tick %d\n", _tick);
  timer_load(TIMER_INTERVAL);
}