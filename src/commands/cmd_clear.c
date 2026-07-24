#include "stdio.h"

void cmd_clear(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf(
        "\033[2J\033[H");
}