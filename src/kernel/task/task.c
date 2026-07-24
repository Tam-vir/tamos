#include "task.h"

#include "scheduler.h"
#include "timer.h"

#include "panic.h"
#include "string.h"
#include "stdio.h"

    static task_t tasks[MAX_TASKS];

static uint32_t next_pid = 1;

static task_t *current_task = 0;

static void task_trampoline(void)
{
    task_t *task =
        task_current();

    task->entry();

    task_exit();
}

void task_init(void)
{
    for (int i = 0;
         i < MAX_TASKS;
         i++)
    {
        tasks[i].state =
            TASK_UNUSED;

        tasks[i].pid = 0;

        tasks[i].wake_tick = 0;

        list_init(
            &tasks[i].node);
    }
}

static task_t *task_alloc(void)
{
    for (int i = 0;
         i < MAX_TASKS;
         i++)
    {
        if (tasks[i].state ==
            TASK_UNUSED)
        {
            return &tasks[i];
        }
    }

    return 0;
}

task_t *task_create(
    const char *name,
    void (*entry)(void))
{
    task_t *task =
        task_alloc();

    if (!task)
    {
        panic(
            "No free task slots");
    }

    kmemset(
        task,
        0,
        sizeof(task_t));

    task->pid =
        next_pid++;

    task->state =
        TASK_READY;

    task->wake_tick = 0;

    task->entry =
        entry;

    kstrncpy(
        task->name,
        name,
        sizeof(task->name) - 1);

    list_init(
        &task->node);

     

    uint64_t sp =
        (uint64_t)&task->stack[TASK_STACK_SIZE];

     

    sp &= ~0xFULL;

    task->context.sp =
        sp;

    task->context.ra =
        (uint64_t)task_exit;

    task->context.mepc =
        (uint64_t)
            task_trampoline;

     

    task->context.mstatus =
        (3ULL << 11) |
        (1ULL << 7);

    scheduler_add(
        task);

    return task;
}

task_t *task_get(
    uint32_t pid)
{
    for (int i = 0;
         i < MAX_TASKS;
         i++)
    {
        if (tasks[i].pid ==
            pid)
        {
            return &tasks[i];
        }
    }

    return 0;
}

task_t *task_current(void)
{
    return current_task;
}

void task_set_current(
    task_t *task)
{
    current_task =
        task;
}

task_t *task_table(void)
{
    return tasks;
}

void task_sleep(
    uint64_t ticks)
{
    if (!current_task)
    {
        return;
    }

    current_task->wake_tick =
        timer_get_ticks() +
        ticks;

    current_task->state =
        TASK_SLEEPING;

    scheduler_yield();
}

void task_exit(void)
{
    if (!current_task)
    {
        panic(
            "No current task");
    }

     

    scheduler_remove(
        current_task);

     

    current_task->state =
        TASK_ZOMBIE;

     

    scheduler_yield();

    // Should never reach here

    while (1)
    {
        asm volatile(
            "wfi");
    }
}
