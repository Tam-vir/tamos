

#include "scheduler.h"

#include "panic.h"
#include "context.h"
#include "task.h"
#include "timer.h"
#include "stdio.h"

static list_node_t ready_queue;

static task_t *current = 0;

void scheduler_init(void)
{
    list_init(
        &ready_queue);

    current = 0;
}

void scheduler_add(
    task_t *task)
{
    if (!task)
    {
        panic(
            "scheduler_add NULL");
    }

     

    if (task->state ==
            TASK_ZOMBIE ||
        task->state ==
            TASK_UNUSED)
    {
        return;
    }

     

    if (task->node.next !=
            &task->node ||
        task->node.prev !=
            &task->node)
    {
        list_remove(
            &task->node);
    }

    task->state =
        TASK_READY;

    list_push_back(
        &ready_queue,
        &task->node);
}

void scheduler_remove(
    task_t *task)
{
    if (!task)
    {
        return;
    }

    if (task->node.next !=
            &task->node ||
        task->node.prev !=
            &task->node)
    {
        list_remove(
            &task->node);
    }
}

void scheduler_wakeup_tasks(void)
{
    uint64_t ticks =
        timer_get_ticks();

    task_t *tasks =
        task_table();

    for (uint32_t i = 0;
         i < MAX_TASKS;
         i++)
    {
        if (tasks[i].state ==
            TASK_SLEEPING)
        {
            if (ticks >=
                tasks[i].wake_tick)
            {
                scheduler_add(
                    &tasks[i]);
            }
        }
    }
}

task_t *scheduler_next(void)
{
     

    list_node_t *node;
    list_node_t *tmp;

    for (node = ready_queue.next;
         node != &ready_queue;)
    {
        tmp =
            node->next;

        task_t *task =
            container_of(
                node,
                task_t,
                node);

        if (task->state ==
                TASK_ZOMBIE ||
            task->state ==
                TASK_UNUSED)
        {
            list_remove(
                node);
        }

        node = tmp;
    }

    node =
        list_front(
            &ready_queue);

    if (!node)
    {
        return 0;
    }

    list_remove(
        node);

    task_t *task =
        container_of(
            node,
            task_t,
            node);

    if (task->state ==
            TASK_ZOMBIE ||
        task->state ==
            TASK_UNUSED)
    {
        return scheduler_next();
    }

    return task;
}

void scheduler_tick(
    cpu_context_t *ctx)
{
    if (!ctx)
    {
        return;
    }

     

    scheduler_wakeup_tasks();

     

    if (!current)
    {
        task_t *next =
            scheduler_next();

        if (!next)
        {
            panic(
                "No runnable task");
        }

        current =
            next;

        current->state =
            TASK_RUNNING;

        task_set_current(
            current);

        *ctx =
            current->context;

        return;
    }

     

    current->context =
        *ctx;

     

    if (current->state ==
        TASK_RUNNING)
    {
        scheduler_add(
            current);
    }

     

    task_t *next =
        scheduler_next();

    if (!next)
    {
        panic(
            "No ready tasks");
    }

    current =
        next;

    current->state =
        TASK_RUNNING;

    task_set_current(
        current);

     

    *ctx =
        current->context;
}

void scheduler_yield(void)
{
    asm volatile(
        "ecall");
}

void scheduler_start(void)
{
    task_t *next =
        scheduler_next();

    if (!next)
    {
        panic(
            "No runnable task");
    }

    current =
        next;

    current->state =
        TASK_RUNNING;

    task_set_current(
        current);

    asm volatile(
        "mv sp,%0\n"
        "jr %1"
        :
        : "r"(current->context.sp),
          "r"(current->context.mepc)
        : "memory");
}

task_t *scheduler_current(void)
{
    return current;
}
