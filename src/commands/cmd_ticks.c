#include "stdio.h"
#include "timer.h"

void cmd_ticks(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf(
        "Ticks: %u\n",
        timer_get_ticks());
}