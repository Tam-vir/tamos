#include "stdio.h"

void cmd_cpuinfo(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf(
        "CPU : RV64\n");

    kprintf(
        "ISA : RV64IMAC\n");

    kprintf(
        "OS  : TamOS v0.1\n");
}