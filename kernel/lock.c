#include "os.h"

int spin_lock() {
  w_mstatus(r_mstatus() & ~MSTATUS_MIE); // Disable interrupts
  return 0;
}
int spin_unlock() {
  w_mstatus(r_mstatus() | MSTATUS_MIE);
  return 0;
}