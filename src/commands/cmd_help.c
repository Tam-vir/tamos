#include "command.h"
#include "stdio.h"

extern command_t commands[];

void cmd_help(
    int argc,
    char **argv)
{

    (void)argc;
    (void)argv;

    kprintf(
        "Available commands:\n");

    kprintf(
        " help\n");

    kprintf(
        " echo\n");

    kprintf(
        " time\n");

    kprintf(
        " ticks\n");

    kprintf(
        " cpuinfo\n");

    kprintf(
        " mem\n");

    kprintf(
        " clear\n");

    kprintf(
        " exit\n");
}