#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "stdio.h"
#include "console.h"

void kprint_char(char c)
{
    console_putc(c);
}

void kprint_string(const char *str)
{
    if (!str)
        return;
    while (*str)
    {
        console_putc(*str++);
    }
}

void kprint_uint64(uint64_t value)
{
    char buffer[32];
    int i = 0;
    if (value == 0)
    {
        console_putc('0');
        return;
    }
    while (value)
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    while (i)
    {
        console_putc(buffer[--i]);
    }
}

void kprint_int64(int64_t value)
{
    if (value < 0)
    {
        console_putc('-');
        value = -value;
    }
    kprint_uint64((uint64_t)value);
}

void kprint_hex(uint64_t value)
{
    char buffer[32];
    int i = 0;
    if (value == 0)
    {
        console_putc('0');
        return;
    }
    while (value)
    {
        uint8_t digit = value & 0xF;
        if (digit < 10)
        {
            buffer[i++] = '0' + digit;
        }
        else
        {
            buffer[i++] = 'A' + digit - 10;
        }
        value >>= 4;
    }
    while (i)
    {
        console_putc(buffer[--i]);
    }
}

void kprint_binary(uint64_t value)
{
    int started = 0;
    for (int i = 63; i >= 0; i--)
    {
        uint64_t bit = (value >> i) & 1;
        if (bit)
            started = 1;
        if (started)
        {
            console_putc(bit ? '1' : '0');
        }
    }
    if (!started)
    {
        console_putc('0');
    }
}

void kvprintf(const char *fmt, va_list args)
{
    while (*fmt)
    {
        if (*fmt != '%')
        {
            console_putc(*fmt++);
            continue;
        }
        fmt++;
        if (*fmt == 'c')
        {
            char value = (char)va_arg(args, int);
            kprint_char(value);
        }
        else if (*fmt == 's')
        {
            char *value = va_arg(args, char *);
            kprint_string(value);
        }
        else if (*fmt == 'd')
        {
            int64_t value = va_arg(args, int64_t);
            kprint_int64(value);
        }
        else if (*fmt == 'u')
        {
            uint64_t value = va_arg(args, uint64_t);
            kprint_uint64(value);
        }
        else if (*fmt == 'x')
        {
            uint64_t value = va_arg(args, uint64_t);
            kprint_hex(value);
        }
        else if (*fmt == 'b')
        {
            uint64_t value = va_arg(args, uint64_t);
            kprint_binary(value);
        }
        else if (*fmt == '%')
        {
            console_putc('%');
        }
        else
        {
            console_putc('%');
            console_putc(*fmt);
        }
        fmt++;
    }
}

void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    kvprintf(fmt, args);
    va_end(args);
}

void kscanf(char *buffer, size_t size)
{
    console_gets(buffer, size);
}