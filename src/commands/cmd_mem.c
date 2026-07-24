#include "memory.h"
#include "stdio.h"
#include <stdint.h>
void cmd_mem(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf("\n");

    kprintf(
        "========== MEMORY ==========\n");

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
        "Heap Total   : %u bytes\n",
        (uint64_t)memory_total());

    kprintf(
        "Heap Used    : %u bytes\n",
        (uint64_t)memory_used());

    kprintf(
        "Heap Free    : %u bytes\n",
        (uint64_t)memory_free());

    kprintf(
        "============================\n\n");
}