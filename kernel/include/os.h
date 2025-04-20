#ifndef __OS_H__
#define __OS_H__

#include "platform.h"
#include "types.h"

#include <stddef.h>
#include <stdarg.h>

// uart
extern int uart_putc(int c);
extern void uart_puts(const char* s);
extern void uart_getc(char* c);

//memory management
extern void *page_alloc(int npages);
extern void page_free(void *p);

// printf
extern int  printf(const char* s, ...);
extern void panic(char *s);

#endif