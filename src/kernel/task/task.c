

#include "task.h"

#include "scheduler.h"
#include "panic.h"
#include "string.h"
#include "timer.h"

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

    task->state =
        TASK_READY;

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

    current_task->wait_entry.data =
        current_task;

    waitqueue_add(
        queue,
        &current_task->wait_entry);

    current_task->state =
        TASK_BLOCKED;

    scheduler_remove(
        current_task);

    // We're safely queued and marked BLOCKED now, so it's safe to
    // let interrupts back in. A wakeup racing in right here just
    // finds us already on the wait queue and unblocks us normally
    // — it can't be lost, unlike if we unlocked before enqueuing.
    if (lock)
    {
        spinlock_unlock(lock);
    }

    scheduler_yield();

    // We've been woken and rescheduled. Re-acquire so callers can
    // treat task_block() like an ordinary "wait under this lock".
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