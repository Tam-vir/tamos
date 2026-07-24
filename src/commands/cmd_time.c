#include "stdio.h"
#include "timer.h"

void cmd_time(
    int argc,
    char **argv)
{
    (void)argc;
    (void)argv;

    kprintf(
        "Uptime: %u seconds\n",
        timer_get_seconds());
}