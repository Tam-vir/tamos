#include "stdio.h"

void cmd_exit(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf(
        "CPU halted.\n");

    while (1)
    {
        asm volatile("wfi");
    }
}