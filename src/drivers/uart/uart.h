#ifndef UART_H
#define UART_H

#include <stddef.h>

void uart_init(void);
void uart_enable_interrupts(void);
void uart_putc(char c);
void uart_puts(const char *str);
char uart_getc(void);
void uart_gets(char *buffer, size_t max_len);
void uart_interrupt_handler(void);

#endif