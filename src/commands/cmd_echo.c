#include "stdio.h"

void cmd_echo(
    int argc,
    char **argv)
{

    for (int i = 1; i < argc; i++)
    {
        kprintf(
            "%s ",
            argv[i]);
    }

    kprintf("\n");
}