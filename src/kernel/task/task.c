#include "task.h"

#include "scheduler.h"
#include "panic.h"
#include "string.h"
#include "timer.h"
#include "fd.h"

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
        tasks[i].pid = 0;

        tasks[i].state =
            TASK_UNUSED;

        tasks[i].priority =
            TASK_PRIORITY_NORMAL;

        tasks[i].effective_priority =
            TASK_PRIORITY_NORMAL;

        tasks[i].ready_since_tick = 0;

        tasks[i].wake_tick = 0;

        list_init(
            &tasks[i].node);

        list_init(
            &tasks[i].wait_entry.node);
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
    return task_create_priority(
        name,
        entry,
        TASK_PRIORITY_NORMAL);
}

task_t *task_create_priority(
    const char *name,
    void (*entry)(void),
    uint8_t priority)
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

    fd_init_task(
        task);

    task->pid =
        next_pid++;

    task->state =
        TASK_READY;

    if (priority >=
        TASK_PRIORITY_LEVELS)
    {
        priority =
            TASK_PRIORITY_LOW;
    }

    task->priority =
        priority;

    
    
    task->effective_priority =
        priority;

    task->entry =
        entry;

    task->wake_tick =
        0;

    kstrncpy(
        task->name,
        name,
        sizeof(task->name) - 1);

    list_init(
        &task->node);

    list_init(
        &task->wait_entry.node);

    uint64_t sp =
        (uint64_t)&task->stack[TASK_STACK_SIZE];

    sp &= ~0xFULL;

    task->context.sp =
        sp;

    task->context.ra =
        (uint64_t)
            task_exit;

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

void task_set_priority(
    task_t *task,
    uint8_t priority)
{
    if (!task)
    {
        return;
    }

    if (priority >=
        TASK_PRIORITY_LEVELS)
    {
        priority =
            TASK_PRIORITY_LOW;
    }

    
    
    
    
    
    if (task->state ==
        TASK_READY)
    {
        scheduler_remove(task);

        task->priority =
            priority;

        scheduler_add(task);
    }
    else
    {
        task->priority =
            priority;
    }
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
    current_task = task;
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
        timer_get_ticks() + ticks;

    current_task->state =
        TASK_SLEEPING;

    scheduler_yield();
}

void task_wakeup(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    if (task->state !=
        TASK_SLEEPING)
    {
        return;
    }

    task->wake_tick = 0;

    scheduler_add(
        task);
}

void task_kill(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    scheduler_remove(
        task);

    task->state =
        TASK_ZOMBIE;
}

void task_destroy(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    scheduler_remove(
        task);

    kmemset(
        task,
        0,
        sizeof(task_t));

    task->state =
        TASK_UNUSED;

    list_init(
        &task->node);
}

void task_wait(
    uint32_t pid)
{
    task_t *task =
        task_get(pid);

    if (!task)
    {
        return;
    }

    while (task->state !=
           TASK_ZOMBIE)
    {
        scheduler_yield();
    }
}

void task_cleanup(void)
{
    for (int i = 0;
         i < MAX_TASKS;
         i++)
    {
        if (tasks[i].state ==
            TASK_ZOMBIE)
        {
            task_destroy(
                &tasks[i]);
        }
    }
}

void task_exit(void)
{
    if (!current_task)
    {
        panic(
            "No current task");
    }

    fd_close_all(
        current_task);

    current_task->state =
        TASK_ZOMBIE;

    scheduler_remove(
        current_task);

    scheduler_yield();

    while (1)
    {
        asm volatile(
            "wfi");
    }
}

void task_block(
    wait_queue_t *queue,
    spinlock_t *lock)
{
    if (!current_task || !queue)
    {
        return;
    }

    list_init(
        &current_task->wait_entry.node);

    current_task->wait_entry.data =
        current_task;

    waitqueue_add(
        queue,
        &current_task->wait_entry);

    current_task->state =
        TASK_BLOCKED;

    scheduler_remove(
        current_task);

    if (lock)
    {
        spinlock_unlock(lock);
    }

    scheduler_yield();

    if (lock)
    {
        spinlock_lock(lock);
    }
}

void task_unblock(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    if (task->state !=
        TASK_BLOCKED)
    {
        return;
    }

    scheduler_add(task);
}

void task_wakeup_queue(
    wait_queue_t *queue)
{
    if (!queue)
    {
        return;
    }

    wait_entry_t *entry;

    while ((entry =
                waitqueue_wake_one(
                    queue)) != 0)
    {
        task_t *task =
            (task_t *)entry->data;

        task_unblock(
            task);
    }
}