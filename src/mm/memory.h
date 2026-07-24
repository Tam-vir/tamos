#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>



void memory_init(void);



uintptr_t memory_start(void);

uintptr_t memory_end(void);

uintptr_t heap_start(void);

uintptr_t heap_end(void);



size_t memory_total(void);

size_t memory_used(void);

size_t memory_free(void);



void memory_dump(void);

#endif