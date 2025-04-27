#include "os.h"
#define DELAY 1000

void task1(void) {
  while (1) {
    uart_puts("Task 1 is running.\n");
    task_delay(DELAY);
    task_yield();
  }
}
void task2(void) {
  while (1) {
    uart_puts("Task 2 is running.\n");
    task_delay(DELAY);
    task_yield();
  }
}

void os_main(void) {

  task_create(task1);
  task_create(task2);
}