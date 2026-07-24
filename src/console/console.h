#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>

void console_putc(char c);

void console_puts(
    const char *str);

void console_clear(void);


char console_getc(void);

void console_gets(
    char *buffer,
    size_t size);

void console_init(void);

#endif