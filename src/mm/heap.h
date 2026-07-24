#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct heap_block
{
    size_t size;

    int free;

    struct heap_block *next;

} heap_block_t;

void heap_init(void);

void *heap_alloc(size_t size);

void heap_free(void *ptr);

size_t heap_block_size(void *ptr);

 

size_t heap_total(void);

size_t heap_used(void);

size_t heap_available_size(void);

size_t heap_largest_free_block(void);

size_t heap_free_blocks(void);

void heap_dump(void);

#endif