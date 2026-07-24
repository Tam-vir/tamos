#include "panic.h"

#include "stdio.h"





void halt(void)
{
    while (1)
    {
        asm volatile("wfi");
    }
}





void warnf(
    const char *fmt,
    ...)
{
    va_list args;

    va_start(
        args,
        fmt);

    kprintf("\n");

    kprintf(
        "-------------------------------------\n");

    kprintf(
        "            KERNEL WARNING\n");

    kprintf(
        "-------------------------------------\n");

    kvprintf(
        fmt,
        args);

    kprintf("\n");

    kprintf(
        "Ticks   : %u\n",
        (uint64_t)timer_get_ticks());

    kprintf(
        "Seconds : %u\n",
        (uint64_t)timer_get_seconds());

    kprintf(
        "-------------------------------------\n\n");

    va_end(args);
}





void panic(
    const char *message)
{
    kprintf("\n");

    kprintf(
        "=====================================\n");

    kprintf(
        "            KERNEL PANIC\n");

    kprintf(
        "=====================================\n");

    kprintf(
        "Message : %s\n",
        message);

    kprintf(
        "Ticks   : %u\n",
        (uint64_t)timer_get_ticks());

    kprintf(
        "Seconds : %u\n",
        (uint64_t)timer_get_seconds());

    kprintf(
        "=====================================\n");

    kprintf(
        "System Halted.\n");

    kprintf(
        "=====================================\n");

    halt();
}





void panicf(
    const char *fmt,
    ...)
{
    va_list args;

    va_start(
        args,
        fmt);

    kprintf("\n");

    kprintf(
        "=====================================\n");

    kprintf(
        "            KERNEL PANIC\n");

    kprintf(
        "=====================================\n");

    kvprintf(
        fmt,
        args);

    kprintf("\n");

    kprintf(
        "Ticks   : %u\n",
        (uint64_t)timer_get_ticks());

    kprintf(
        "Seconds : %u\n",
        (uint64_t)timer_get_seconds());

    kprintf(
        "=====================================\n");

    kprintf(
        "System Halted.\n");

    kprintf(
        "=====================================\n");

    va_end(args);

    halt();
}