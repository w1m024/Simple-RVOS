#include "os.h"

#include "user_api.h"

#define DELAY 4000

void user_task0(void) {
  uart_puts("Task0: Created\n");
  unsigned int hid = -1;

#ifdef CONFIG_SYSCALL
  int ret = -1;
  ret = gethid(&hid);
  // ret = gethid(NULL);
  if (!ret) {
    printf("system call returned!, hart id is %d\n", hid);
  } else {
    printf("gethid() failed, return: %d\n", ret);
  }
#endif

  while (1) {
    uart_puts("Task0: Running\n");
    task_delay(DELAY);
  }
}

void user_task1(void) {
  uart_puts("Task1: Created\n");
  while (1) {
    uart_puts("Task1: Running\n");
    task_delay(DELAY);
  }
}

void os_main(void) {
  task_create(user_task0);
  task_create(user_task1);
}