#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>



void kprint_char(char c);

void kprint_string(
    const char *str);

void kprint_uint64(
    uint64_t value);

void kprint_int64(
    int64_t value);

void kprint_hex(
    uint64_t value);

void kprint_binary(
    uint64_t value);



void kvprintf(
    const char *fmt,
    va_list args);

void kprintf(
    const char *fmt,
    ...);

void kscanf(
    char *buffer,
    size_t size);

#endif