#ifndef BITMAP_H
#define BITMAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t *bits;

    size_t size;

} bitmap_t;

void bitmap_init(
    bitmap_t *map,
    uint8_t *buffer,
    size_t bits);

void bitmap_set(
    bitmap_t *map,
    size_t index);

void bitmap_clear(
    bitmap_t *map,
    size_t index);

int bitmap_test(
    bitmap_t *map,
    size_t index);

int bitmap_find_free(
    bitmap_t *map);
    
int bitmap_find_free_range(
    bitmap_t *map,
    size_t count);
#endif