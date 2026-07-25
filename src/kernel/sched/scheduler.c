#include "scheduler.h"

#include "panic.h"
#include "context.h"
#include "task.h"
#include "timer.h"
#include "stdio.h"

static list_node_t ready_queues[TASK_PRIORITY_LEVELS];

static task_t *current = 0;

void scheduler_init(void)
{
    for (int i = 0;
         i < TASK_PRIORITY_LEVELS;
         i++)
    {
        list_init(
            &ready_queues[i]);
    }

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

    
    
    
    
    
    
    uint8_t priority =
        task->priority;

    if (priority >=
        TASK_PRIORITY_LEVELS)
    {
        priority =
            TASK_PRIORITY_LOW;
    }

    task->effective_priority =
        priority;

    task->ready_since_tick =
        timer_get_ticks();

    list_push_back(
        &ready_queues[priority],
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

void scheduler_age_tasks(void)
{
    uint64_t now =
        timer_get_ticks();

    task_t *tasks =
        task_table();

    for (uint32_t i = 0;
         i < MAX_TASKS;
         i++)
    {
        task_t *task =
            &tasks[i];

        if (task->state !=
            TASK_READY)
        {
            continue;
        }

        
        if (task->effective_priority ==
            TASK_PRIORITY_HIGH)
        {
            continue;
        }

        if (now - task->ready_since_tick <
                      TASK_AGING_TICKS)
        {
            continue;
        }

        
        
        
        list_remove(
            &task->node);

        task->effective_priority--;

        
        
        
        task->ready_since_tick =
            now;

        list_push_back(
            &ready_queues[task->effective_priority],
            &task->node);
    }
}

static void
scheduler_cleanup_queue(
    list_node_t *queue)
{
    list_node_t *node;
    list_node_t *tmp;

    for (node = queue->next;
         node != queue;)
    {
        tmp = node->next;

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
            list_remove(node);
        }

        node = tmp;
    }
}

task_t *scheduler_next(void)
{
    for (int i = 0;
         i < TASK_PRIORITY_LEVELS;
         i++)
    {
        scheduler_cleanup_queue(
            &ready_queues[i]);
    }

    for (int i = 0;
         i < TASK_PRIORITY_LEVELS;
         i++)
    {
        list_node_t *node =
            list_front(
                &ready_queues[i]);

        if (!node)
        {
            continue;
        }

        list_remove(node);

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
            continue;
        }

        return task;
    }

    return 0;
}

void scheduler_tick(
    cpu_context_t *ctx)
{
    if (!ctx)
    {
        return;
    }

    scheduler_wakeup_tasks();

    
    
    
    
    scheduler_age_tasks();

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