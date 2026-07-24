#include <stdint.h>
#include <stddef.h>

#include "memory.h"
#include "stdio.h"
#include "logger.h"

    

     

    extern char _memory_start;
extern char _memory_end;

extern char _heap_start;
extern char _heap_end;



uintptr_t memory_start(void)
{
    return (uintptr_t)&_memory_start;
}

uintptr_t memory_end(void)
{
    return (uintptr_t)&_memory_end;
}

uintptr_t heap_start(void)
{
    return (uintptr_t)&_heap_start;
}

uintptr_t heap_end(void)
{
    return (uintptr_t)&_heap_end;
}



size_t memory_total(void)
{
    return heap_end() - heap_start();
}

size_t memory_used(void)
{
     

    return 0;
}

size_t memory_free(void)
{
    return memory_total() - memory_used();
}



void memory_dump(void)
{
    kprintf("\n");

    kprintf(
        "=========== MEMORY MAP ===========\n");

    kprintf(
        "Memory Start : 0x%x\n",
        (uint64_t)memory_start());

    kprintf(
        "Memory End   : 0x%x\n",
        (uint64_t)memory_end());

    kprintf(
        "Heap Start   : 0x%x\n",
        (uint64_t)heap_start());

    kprintf(
        "Heap End     : 0x%x\n",
        (uint64_t)heap_end());

    kprintf(
        "Heap Size    : %u bytes\n",
        (uint64_t)memory_total());

    kprintf(
        "Heap Used    : %u bytes\n",
        (uint64_t)memory_used());

    kprintf(
        "Heap Free    : %u bytes\n",
        (uint64_t)memory_free());

    kprintf(
        "=================================\n\n");
}



void memory_init(void)
{
    klog_info(
        "Initializing memory subsystem.");

    memory_dump();

    klog_info(
        "Memory subsystem initialized.");
}
