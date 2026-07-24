

#include <stdint.h>

#include "timer.h"
#include "trap.h"
#include "clint.h"
#include "interrupt.h"
#include "task.h"

volatile uint64_t tick_count = 0;

volatile uint64_t next_timer_tick = 0;

void timer_interrupt_handler(void)
{
    uint64_t now =
        clint_read_mtime();

     

    next_timer_tick =
        now + TIMER_INTERVAL;

    clint_write_mtimecmp(
        next_timer_tick);

     

    if (!kernel_ready)
    {
        return;
    }

     

    tick_count++;

     

    task_t *tasks =
        task_table();

    for (uint32_t i = 0;
         i < MAX_TASKS;
         i++)
    {
        task_t *task =
            &tasks[i];

        if (task->state !=
            TASK_SLEEPING)
        {
            continue;
        }

        if (tick_count >=
            task->wake_tick)
        {
            task_wakeup(
                task);
        }
    }

     

    task_cleanup();
}

void timer_boot_init(void)
{
    uint64_t now =
        clint_read_mtime();

    next_timer_tick =
        now + TIMER_INTERVAL;

    clint_write_mtimecmp(
        next_timer_tick);

     

    interrupt_register(
        INTERRUPT_MTI,
        timer_interrupt_handler);
}

void timer_enable(void)
{
     

    interrupt_enable_irq(
        INTERRUPT_MTI);

     

    interrupt_enable();
}

uint64_t timer_get_ticks(void)
{
    return tick_count;
}

uint64_t timer_get_seconds(void)
{
     

    return tick_count / 10;
}