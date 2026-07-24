#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

 

void *malloc(
    size_t size);

void free(
    void *ptr);

void *calloc(
    size_t count,
    size_t size);

void *realloc(
    void *ptr,
    size_t size);

 

int atoi(
    const char *str);

long atol(
    const char *str);

 

int abs(
    int value);

void exit(
    int status);

#endif